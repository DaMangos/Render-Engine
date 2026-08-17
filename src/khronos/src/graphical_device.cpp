#include "detail/make_shared_with_data.hpp"
#include "vulkan/vulkan.hpp"

#include <khronos/graphical_device.hpp>
#include <khronos/graphics_pipeline.hpp>
#include <khronos/library.hpp>
#include <khronos/memory_buffer.hpp>
#include <khronos/present_window.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>
#include <logging/serialize.hpp>

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_to_string.hpp>

#include <algorithm>
#include <cassert>
#include <expected>
#include <limits>
#include <memory>
#include <ranges>
#include <string_view>
#include <vector>

namespace
{
namespace detail
{
[[nodiscard]]
static std::expected<std::vector<char const *>, std::vector<char const *>> get_required_device_extensions(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device)
{
  auto const is_extension_available
    = [properties = physical_device->enumerateDeviceExtensionProperties()](std::string_view const extension)
  {
    return std::ranges::any_of(properties, [&](auto const & property) { return property.extensionName == extension; });
  };

  auto required_device_extensions = std::vector{vk::KHRSwapchainExtensionName, vk::KHRSynchronization2ExtensionName};

  auto unavailable_device_extensions = required_device_extensions;

  std::erase_if(unavailable_device_extensions, is_extension_available);

  if(not unavailable_device_extensions.empty())
    return std::unexpected{unavailable_device_extensions};

  auto const & [physical_device_properties, physical_device_properties_12]
    = physical_device->getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVulkan12Properties>();

  if(is_extension_available(vk::KHRPortabilitySubsetExtensionName))
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data() << ") is not fully compliant";

    required_device_extensions.emplace_back(vk::KHRPortabilitySubsetExtensionName);
  }

  return required_device_extensions;
}

template <typename... Features>
[[nodiscard]]
static std::vector<std::size_t> find_unavailable_feature_indices(
  std::shared_ptr<vk::raii::PhysicalDevice const> const &              physical_device,
  vk::StructureChain<vk::PhysicalDeviceFeatures2, Features...> const & requested_features)
{
  constexpr auto flatten = [](vk::PhysicalDeviceFeatures2 const & feature, Features const &... features)
  {
    constexpr auto remove_headers = [](auto const & structure)
    {
      return [&]<std::size_t... I>(std::index_sequence<I...>)
      {
        return std::forward_as_tuple(std::get<I + 2>(structure)...);
      }(std::make_index_sequence<std::tuple_size_v<std::decay_t<decltype(structure)>> - 2>());
    };

    return std::tuple_cat(feature.features.reflect(), remove_headers(features.reflect())...);
  };

  constexpr auto to_bitset = [](auto const & flags)
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>)
    {
      std::bitset<std::tuple_size_v<std::decay_t<decltype(flags)>>> bits;

      (bits.set(I, std::get<I>(flags) == vk::True), ...);

      return bits;
    }(std::make_index_sequence<std::tuple_size_v<std::decay_t<decltype(flags)>>>());
  };

  auto const supported_features = physical_device->getFeatures2<vk::PhysicalDeviceFeatures2, Features...>();

  auto const requested_bits = to_bitset(std::apply(flatten, requested_features));

  auto const supported_bits = to_bitset(std::apply(flatten, supported_features));

  auto const unavailable_bits = requested_bits & ~supported_bits;

  std::vector<std::size_t> unavailable_features_indices;

  unavailable_features_indices.reserve(unavailable_bits.count());

  for(std::size_t unavailable_index = 0, requested_index = 0; unavailable_index < unavailable_bits.size();
      ++unavailable_index)
  {
    if(unavailable_bits[unavailable_index])
      unavailable_features_indices.emplace_back(requested_index);

    if(requested_bits[unavailable_index])
      requested_index++;
  }

  return unavailable_features_indices;
}

[[nodiscard]]
static int score(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device)
{
  if(not physical_device)
    return 0;

  auto const is_extension_unavailable
    = [properties = physical_device->enumerateDeviceExtensionProperties()](std::string_view const extension)
  {
    return std::ranges::none_of(properties, [&](auto const & property) { return property.extensionName == extension; });
  };

  int score = 0;

  if(is_extension_unavailable(vk::KHRPortabilitySubsetExtensionName))
    score += 1;

  return score;
}
}
}

khronos::graphics_pipeline khronos::graphical_device::create_graphics_pipeline() const
{
  return {device, default_swapchain_create_info};
}

khronos::staging_buffer khronos::graphical_device::allocate_staging_buffer(vk::DeviceSize const size) const
{
  return {
    physical_device,
    device,
    transfer_and_graphics_and_present_queue,
    transfer_and_graphics_and_present_queue_family_index,
    size,
  };
}

khronos::index_transfer_buffer khronos::graphical_device::allocate_index_transfer_buffer(
  vk::DeviceSize const size) const
{
  return {physical_device, device, size};
}

khronos::vertex_transfer_buffer khronos::graphical_device::allocate_vertex_transfer_buffer(
  vk::DeviceSize const size) const
{
  return {physical_device, device, size};
}

khronos::render_window khronos::graphical_device::create_render_window(present_window && present_window) const
{
  return {
    std::move(present_window),
    physical_device,
    device,
    default_swapchain_create_info,
    transfer_and_graphics_and_present_queue,
    transfer_and_graphics_and_present_queue_family_index,
  };
}

void khronos::graphical_device::flush(staging_buffer & staging_buffer)
{
  staging_buffer.transfers.splice(staging_buffer.transfers.end(),
                                  staging_buffer.transfers,
                                  staging_buffer.transfers.begin());

  auto const transfer = staging_buffer.transfers.front();

  auto const wait_for_fences_result
    = device->waitForFences(**transfer.fence, vk::True, std::numeric_limits<std::uint64_t>::max());

  if(wait_for_fences_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(wait_for_fences_result), "failed to wait for fence"};

  if(wait_for_fences_result > vk::Result::eSuccess)
    logging::warning() << "wait for fences returned a warning: " << vk::to_string(wait_for_fences_result);

  device->resetFences(**transfer.fence);

  auto const command_buffer_begin_info
    = vk::CommandBufferBeginInfo{}.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

  transfer.command_buffer->begin(command_buffer_begin_info);

  for(auto const & region : staging_buffer.regions)
  {
    auto const buffer_region = vk::BufferCopy2{}.setSrcOffset(region.offset).setSize(region.size);

    auto const copy_buffer_info = vk::CopyBufferInfo2{}
                                    .setDstBuffer(*region.destination_buffer)
                                    .setSrcBuffer(*staging_buffer.buffer)
                                    .setRegions(buffer_region);

    transfer.command_buffer->copyBuffer2(copy_buffer_info);
  }
  transfer.command_buffer->end();

  auto const submit_info = vk::SubmitInfo{}.setCommandBuffers(**transfer.command_buffer);

  transfer_and_graphics_and_present_queue->submit(submit_info, *transfer.fence);
  transfer_and_graphics_and_present_queue->waitIdle();

  staging_buffer.regions.clear();
}

void khronos::graphical_device::draw(graphics_pipeline &      graphics_pipeline,
                                     index_transfer_buffer &  index_transfer_buffer,
                                     vertex_transfer_buffer & vertex_transfer_buffer,
                                     render_window &          render_window) const
{
  render_window.frames.splice(render_window.frames.end(), render_window.frames, render_window.frames.begin());

  auto const frame = render_window.frames.front();

  auto const wait_for_fences_result
    = device->waitForFences(**frame.in_flight_fence, vk::True, std::numeric_limits<std::uint64_t>::max());

  if(wait_for_fences_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(wait_for_fences_result), "failed to wait for fence"};

  if(wait_for_fences_result > vk::Result::eSuccess)
    logging::warning() << "wait for fences returned a warning: " << vk::to_string(wait_for_fences_result);

  device->resetFences(**frame.in_flight_fence);

  auto const & [acquire_next_image_result, next_image_index]
    = render_window.swapchain->acquireNextImage(std::numeric_limits<std::uint64_t>::max(),
                                                *frame.present_complete_semaphores);

  if(acquire_next_image_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(acquire_next_image_result), "failed to acquire next image"};

  if(acquire_next_image_result > vk::Result::eSuccess)
    logging::warning() << "acquiring the next image returned a warning: " << vk::to_string(acquire_next_image_result);

  auto const current_image = render_window.images.at(next_image_index);

  auto const image_subresource_range = vk::ImageSubresourceRange{}
                                         .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                         .setBaseMipLevel(0)
                                         .setLevelCount(1)
                                         .setBaseArrayLayer(0)
                                         .setLayerCount(1);

  auto const write_image_memory_barrier = vk::ImageMemoryBarrier2{}
                                            .setSrcStageMask(vk::PipelineStageFlagBits2::eNone)
                                            .setSrcAccessMask(vk::AccessFlagBits2::eNone)
                                            .setDstStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
                                            .setDstAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite)
                                            .setOldLayout(vk::ImageLayout::eUndefined)
                                            .setNewLayout(vk::ImageLayout::eColorAttachmentOptimal)
                                            .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                                            .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
                                            .setImage(current_image.image)
                                            .setSubresourceRange(image_subresource_range);

  auto const write_dependency_info
    = vk::DependencyInfo{}.setImageMemoryBarrierCount(1).setPImageMemoryBarriers(&write_image_memory_barrier);

  auto const attachmentInfo = vk::RenderingAttachmentInfo{}
                                .setImageView(*current_image.image_view)
                                .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
                                .setLoadOp(vk::AttachmentLoadOp::eClear)
                                .setStoreOp(vk::AttachmentStoreOp::eStore)
                                .setClearValue(vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f));

  auto const viewport = vk::Viewport{}
                          .setX(0.0f)
                          .setY(0.0f)
                          .setWidth(static_cast<float>(render_window.swapchain_create_info->imageExtent.width))
                          .setHeight(static_cast<float>(render_window.swapchain_create_info->imageExtent.height))
                          .setMinDepth(0.0f)
                          .setMaxDepth(1.0f);

  auto const scissor = vk::Rect2D{}
                         .setOffset(vk::Offset2D{}.setX(0).setY(0))
                         .setExtent(render_window.swapchain_create_info->imageExtent);

  auto const rendering_info
    = vk::RenderingInfo{}.setRenderArea(scissor).setLayerCount(1).setColorAttachmentCount(1).setPColorAttachments(
      &attachmentInfo);

  auto const present_image_memory_barrier = vk::ImageMemoryBarrier2{}
                                              .setSrcStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
                                              .setSrcAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite)
                                              .setDstStageMask(vk::PipelineStageFlagBits2::eNone)
                                              .setDstAccessMask(vk::AccessFlagBits2::eNone)
                                              .setOldLayout(vk::ImageLayout::eColorAttachmentOptimal)
                                              .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
                                              .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                                              .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
                                              .setImage(current_image.image)
                                              .setSubresourceRange(image_subresource_range);
  auto const present_dependency_info
    = vk::DependencyInfo{}.setImageMemoryBarrierCount(1).setPImageMemoryBarriers(&present_image_memory_barrier);

  auto const wait_destination_stage_mask = vk::PipelineStageFlags{vk::PipelineStageFlagBits::eColorAttachmentOutput};

  frame.command_buffer->reset();
  frame.command_buffer->begin({});
  frame.command_buffer->pipelineBarrier2(write_dependency_info);
  frame.command_buffer->beginRendering(rendering_info);
  frame.command_buffer->bindPipeline(vk::PipelineBindPoint::eGraphics, **graphics_pipeline.pipeline);
  frame.command_buffer->setViewport(0, viewport);
  frame.command_buffer->setScissor(0, scissor);
  frame.command_buffer->bindIndexBuffer(**index_transfer_buffer.buffer, 0, vk::IndexType::eUint32);
  frame.command_buffer->bindVertexBuffers(0, **vertex_transfer_buffer.buffer, {0});
  frame.command_buffer->drawIndexed(index_transfer_buffer.indices_count, 1, 0, 0, 0);
  frame.command_buffer->endRendering();
  frame.command_buffer->pipelineBarrier2(present_dependency_info);
  frame.command_buffer->end();

  auto const graphics_submit_info = vk::SubmitInfo{}
                                      .setWaitSemaphores(**frame.present_complete_semaphores)
                                      .setWaitDstStageMask(wait_destination_stage_mask)
                                      .setCommandBuffers(**frame.command_buffer)
                                      .setSignalSemaphores(**current_image.render_complete_semaphores);

  transfer_and_graphics_and_present_queue->submit(graphics_submit_info, **frame.in_flight_fence);

  auto const present_info = vk::PresentInfoKHR{}
                              .setWaitSemaphores(**current_image.render_complete_semaphores)
                              .setSwapchains(**render_window.swapchain)
                              .setImageIndices(next_image_index);

  auto const present_result = transfer_and_graphics_and_present_queue->presentKHR(present_info);

  if(present_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(present_result), "failed to present"};

  if(present_result > vk::Result::eSuccess)
    logging::warning() << "presenting returned a warning: " << vk::to_string(present_result);
}

khronos::graphical_device::graphical_device(std::shared_ptr<vk::raii::Context const> const &    context,
                                            std::shared_ptr<vk::raii::Instance const> const &   instance,
                                            std::shared_ptr<vk::raii::SurfaceKHR const> const & surface)
{
  using namespace logging::serialize;

  auto const required_physical_device_features = vk::StructureChain{
    vk::PhysicalDeviceFeatures2{},
    vk::PhysicalDeviceVulkan11Features{}.setShaderDrawParameters(vk::True),
    vk::PhysicalDeviceVulkan13Features{}.setDynamicRendering(vk::True).setSynchronization2(vk::True),
    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT{}.setExtendedDynamicState(vk::True),
  };

  auto const make_shared_physical_device = [&](auto const & physical_device)
  {
    auto const shared_physical_device = detail::make_shared_with_data<vk::raii::PhysicalDevice const>(physical_device);

    detail::emplace_data(shared_physical_device, instance);

    return shared_physical_device;
  };

  auto const physical_devices
    = instance->enumeratePhysicalDevices() | std::views::transform(make_shared_physical_device);

  logging::verbose() << "there are " << physical_devices.size() << " physical devices available";

  for(auto const & potential_physical_device : physical_devices)
  {
    auto const & [physical_device_properties, physical_device_properties_12]
      = potential_physical_device
          ->getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVulkan12Properties>();

    auto const api_version          = context->enumerateInstanceVersion();
    auto const physical_api_version = physical_device_properties.properties.apiVersion;

    if(not(vk::apiVersionMajor(physical_api_version) >= vk::apiVersionMajor(api_version)
           and vk::apiVersionMinor(physical_api_version) >= vk::apiVersionMinor(api_version)))
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                         << physical_device_properties_12.driverName.data()
                         << ") is not suitable because it only supports vulkan api version "
                         << vk::apiVersionMajor(physical_api_version) << '.'
                         << vk::apiVersionMinor(physical_api_version) << '.'
                         << vk::apiVersionPatch(physical_api_version) << " and we require "
                         << vk::apiVersionMajor(api_version) << '.' << vk::apiVersionMinor(api_version) << '.'
                         << vk::apiVersionPatch(api_version);
      continue;
    }

    auto device_extensions = ::detail::get_required_device_extensions(potential_physical_device);

    if(not device_extensions)
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                         << physical_device_properties_12.driverName.data()
                         << ") is not suitable because it's missing physical device extensions: "
                         << device_extensions.error();
      continue;
    }

    logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data()
                       << ") has got all required physical device extensions: " << *device_extensions;

    auto const unavailable_features_indices
      = ::detail::find_unavailable_feature_indices(potential_physical_device, required_physical_device_features);

    if(not unavailable_features_indices.empty())
    {
      if(unavailable_features_indices.size() == 1)
        logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                           << physical_device_properties_12.driverName.data()
                           << ") is not suitable because it's missing " << unavailable_features_indices.size()
                           << " required physical device feature at index " << unavailable_features_indices.front();
      else
        logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                           << physical_device_properties_12.driverName.data()
                           << ") is not suitable because it's missing " << unavailable_features_indices.size()
                           << " required physical device features at indices " << unavailable_features_indices;
      continue;
    }

    logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data() << ") has all requires device features";

    auto const queue_family_properties = potential_physical_device->getQueueFamilyProperties();

    std::uint32_t transfer_queue_family_index = 0;
    for(; transfer_queue_family_index < queue_family_properties.size(); transfer_queue_family_index++)
      if(queue_family_properties[transfer_queue_family_index].queueFlags & vk::QueueFlagBits::eTransfer)
        break;

    if(transfer_queue_family_index == queue_family_properties.size())
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                         << physical_device_properties_12.driverName.data()
                         << ") is not suitable because it's missing the transfer queue family";
      continue;
    }

    std::uint32_t graphics_queue_family_index = 0;
    for(; graphics_queue_family_index < queue_family_properties.size(); graphics_queue_family_index++)
      if(queue_family_properties[graphics_queue_family_index].queueFlags & vk::QueueFlagBits::eGraphics)
        break;

    if(graphics_queue_family_index == queue_family_properties.size())
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                         << physical_device_properties_12.driverName.data()
                         << ") is not suitable because it's missing the graphics queue family";
      continue;
    }

    std::uint32_t present_queue_family_index = 0;
    for(; present_queue_family_index < queue_family_properties.size(); present_queue_family_index++)
      if(potential_physical_device->getSurfaceSupportKHR(present_queue_family_index, *surface))
        break;

    if(present_queue_family_index == queue_family_properties.size())
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                         << physical_device_properties_12.driverName.data()
                         << ") is not suitable because it's missing the present queue family";
      continue;
    }

    if(transfer_queue_family_index != graphics_queue_family_index
       or graphics_queue_family_index != present_queue_family_index
       or present_queue_family_index != transfer_queue_family_index)
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                         << physical_device_properties_12.driverName.data()
                         << ") is not suitable because the transfer queue family index " << transfer_queue_family_index
                         << " and the graphics queue family index " << graphics_queue_family_index
                         << " and the present queue family index " << present_queue_family_index << " are different";
      continue;
    }

    logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data()
                       << ") has transfer, graphics, and present queue family at index " << transfer_queue_family_index;

    if(::detail::score(potential_physical_device) < ::detail::score(physical_device))
      continue;

    physical_device = potential_physical_device;

    auto const surface_capabilities = potential_physical_device->getSurfaceCapabilitiesKHR(*surface);

    auto const min_image_count
      = surface_capabilities.maxImageCount == 0
        ? std::max(3u, surface_capabilities.minImageCount)
        : std::clamp(3u, surface_capabilities.minImageCount, surface_capabilities.maxImageCount);

    default_swapchain_create_info = detail::make_shared_with_data<vk::SwapchainCreateInfoKHR const>(
      vk::SwapchainCreateInfoKHR{}
        .setSurface(*surface)
        .setMinImageCount(min_image_count)
        .setImageArrayLayers(1)
        .setImageSharingMode(vk::SharingMode::eExclusive)
        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
        .setPreTransform(surface_capabilities.currentTransform)
        .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
        .setPresentMode(vk::PresentModeKHR::eFifo)
        .setImageFormat(vk::Format::eB8G8R8A8Srgb)
        .setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear)
        .setClipped(vk::True));

    detail::emplace_data(default_swapchain_create_info, surface);

    transfer_and_graphics_and_present_queue_family_index = transfer_queue_family_index;
  };

  if(not physical_device)
    throw std::runtime_error("there are no suitable physical devices");

  auto const & [physical_device_properties, physical_device_properties_12]
    = physical_device->getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVulkan12Properties>();

  logging::info() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                  << physical_device_properties_12.driverName.data() << ") is suitable";

  constexpr auto queue_priority = 0.5f;

  auto const graphics_and_present_device_queue_create_infos
    = vk::DeviceQueueCreateInfo{}
        .setQueueFamilyIndex(transfer_and_graphics_and_present_queue_family_index)
        .setQueueCount(1)
        .setQueuePriorities(queue_priority);

  auto const required_device_extensions = ::detail::get_required_device_extensions(physical_device);

  auto const & [device_create_info, _] = vk::StructureChain{
    vk::DeviceCreateInfo{}
      .setQueueCreateInfos(graphics_and_present_device_queue_create_infos)
      .setPEnabledExtensionNames(*required_device_extensions),
    required_physical_device_features.get(),
  };

  device = detail::make_shared_with_data<vk::raii::Device const>(*physical_device, device_create_info);

  auto const graphics_and_present_queue_create_info
    = vk::DeviceQueueInfo2{}.setQueueFamilyIndex(transfer_and_graphics_and_present_queue_family_index).setQueueIndex(0);

  detail::emplace_data(device, physical_device);

  transfer_and_graphics_and_present_queue
    = detail::make_shared_with_data<vk::raii::Queue const>(*device, graphics_and_present_queue_create_info);

  detail::emplace_data(transfer_and_graphics_and_present_queue, device);
}