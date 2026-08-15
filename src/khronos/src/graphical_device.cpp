#include "detail/make_shared_with_data.hpp"
#include "vulkan/vulkan_raii.hpp"
#include "vulkan/vulkan_to_string.hpp"

#include <khronos/graphical_device.hpp>
#include <khronos/library.hpp>
#include <khronos/present_window.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>
#include <logging/serialize.hpp>

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
    return std::ranges::any_of(properties,
                               [&](auto const & property) { return property.extensionName == extension; });
  };

  auto required_device_extensions
    = std::vector{vk::KHRSwapchainExtensionName, vk::KHRSynchronization2ExtensionName};

  auto unavailable_device_extensions = required_device_extensions;

  std::erase_if(unavailable_device_extensions, is_extension_available);

  if(not unavailable_device_extensions.empty())
    return std::unexpected{unavailable_device_extensions};

  auto const & [physical_device_properties, physical_device_properties_12]
    = physical_device->getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVulkan12Properties>();

  if(is_extension_available(vk::KHRPortabilitySubsetExtensionName))
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data()
                       << " (" << physical_device_properties_12.driverName.data()
                       << ") is not fully compliant";

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

  auto const is_extension_available
    = [properties = physical_device->enumerateDeviceExtensionProperties()](std::string_view const extension)
  {
    return std::ranges::any_of(properties,
                               [&](auto const & property) { return property.extensionName == extension; });
  };

  int score = 0;

  if(not is_extension_available(vk::KHRPortabilitySubsetExtensionName))
    score += 1;

  return score;
}
}
}

khronos::render_window khronos::graphical_device::create_render_window(present_window && window) const
{
  return {
    std::move(window),
    physical_device,
    device,
    default_swapchain_create_info,
    graphics_and_present_queue,
    graphics_and_present_queue_family_index,
  };
}

void khronos::graphical_device::draw_next_frame(render_window & window) const
{
  auto const current_frame = window.frames.front();

  auto const wait_for_fences_result = device->waitForFences(**current_frame.in_flight_fence,
                                                            vk::True,
                                                            std::numeric_limits<std::uint64_t>::max());

  if(wait_for_fences_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(wait_for_fences_result), "failed to wait for fence"};

  if(wait_for_fences_result > vk::Result::eSuccess)
    logging::warning() << "wait for fences returned a warning: " << vk::to_string(wait_for_fences_result);

  device->resetFences(**current_frame.in_flight_fence);

  auto const & [acquire_next_image_result, next_image_index]
    = window.swapchain->acquireNextImage(std::numeric_limits<std::uint64_t>::max(),
                                         *current_frame.present_complete_semaphores);

  if(acquire_next_image_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(acquire_next_image_result), "failed to acquire next image"};

  if(acquire_next_image_result > vk::Result::eSuccess)
    logging::warning() << "acquiring the next image returned a warning: "
                       << vk::to_string(acquire_next_image_result);

  auto const current_image = window.images.at(next_image_index);

  auto const image_subresource_range = vk::ImageSubresourceRange{}
                                         .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                         .setBaseMipLevel(0)
                                         .setLevelCount(1)
                                         .setBaseArrayLayer(0)
                                         .setLayerCount(1);

  auto const write_image_memory_barrier
    = vk::ImageMemoryBarrier2{}
        .setSrcStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
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
                          .setWidth(static_cast<float>(window.swapchain_create_info->imageExtent.width))
                          .setHeight(static_cast<float>(window.swapchain_create_info->imageExtent.height))
                          .setMinDepth(0.0f)
                          .setMaxDepth(1.0f);

  auto const scissor = vk::Rect2D{}
                         .setOffset(vk::Offset2D{}.setX(0).setY(0))
                         .setExtent(window.swapchain_create_info->imageExtent);

  auto const rendering_info = vk::RenderingInfo{}
                                .setRenderArea(scissor)
                                .setLayerCount(1)
                                .setColorAttachmentCount(1)
                                .setPColorAttachments(&attachmentInfo);

  auto const present_image_memory_barrier
    = vk::ImageMemoryBarrier2{}
        .setSrcStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
        .setSrcAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite)
        .setDstStageMask(vk::PipelineStageFlagBits2::eBottomOfPipe)
        .setDstAccessMask(vk::AccessFlagBits2::eNone)
        .setOldLayout(vk::ImageLayout::eColorAttachmentOptimal)
        .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
        .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
        .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
        .setImage(current_image.image)
        .setSubresourceRange(image_subresource_range);

  auto const present_dependency_info
    = vk::DependencyInfo{}.setImageMemoryBarrierCount(1).setPImageMemoryBarriers(
      &present_image_memory_barrier);

  auto const wait_destination_stage_mask = vk::PipelineStageFlags{
    vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eTopOfPipe,
  };

  current_frame.graphics_and_present_command_buffer->reset();
  current_frame.graphics_and_present_command_buffer->begin({});
  current_frame.graphics_and_present_command_buffer->pipelineBarrier2(write_dependency_info);
  current_frame.graphics_and_present_command_buffer->beginRendering(rendering_info);
  current_frame.graphics_and_present_command_buffer->bindPipeline(vk::PipelineBindPoint::eGraphics,
                                                                  **window.graphics_pipeline);
  current_frame.graphics_and_present_command_buffer->setViewport(0, viewport);
  current_frame.graphics_and_present_command_buffer->setScissor(0, scissor);
  current_frame.graphics_and_present_command_buffer->bindVertexBuffers(0, **window.vertex_buffer, {0});
  current_frame.graphics_and_present_command_buffer
    ->draw(static_cast<uint32_t>(window.vertices.size()), 1, 0, 0);
  current_frame.graphics_and_present_command_buffer->endRendering();
  current_frame.graphics_and_present_command_buffer->pipelineBarrier2(present_dependency_info);
  current_frame.graphics_and_present_command_buffer->end();

  auto const graphics_submit_info = vk::SubmitInfo{}
                                      .setWaitSemaphores(**current_frame.present_complete_semaphores)
                                      .setWaitDstStageMask(wait_destination_stage_mask)
                                      .setCommandBuffers(**current_frame.graphics_and_present_command_buffer)
                                      .setSignalSemaphores(**current_image.render_complete_semaphores);

  graphics_and_present_queue->submit(graphics_submit_info, **current_frame.in_flight_fence);

  auto const present_info = vk::PresentInfoKHR{}
                              .setWaitSemaphores(**current_image.render_complete_semaphores)
                              .setSwapchains(**window.swapchain)
                              .setImageIndices(next_image_index);

  auto const present_result = graphics_and_present_queue->presentKHR(present_info);

  if(present_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(present_result), "failed to present"};

  if(present_result > vk::Result::eSuccess)
    logging::warning() << "presenting returned a warning: " << vk::to_string(present_result);

  window.frames.splice(window.frames.end(), window.frames, window.frames.begin());
}

khronos::graphical_device::graphical_device(std::shared_ptr<vk::raii::Instance const>           instance,
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
    auto const shared_physical_device
      = detail::make_shared_with_data<vk::raii::PhysicalDevice const>(physical_device);

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

    if(physical_device_properties.properties.apiVersion < vk::ApiVersion13)
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data()
                         << " (" << physical_device_properties_12.driverName.data()
                         << ") is not suitable because does not support vulkan api version 1.3";
      continue;
    }

    auto device_extensions = ::detail::get_required_device_extensions(potential_physical_device);

    if(not device_extensions)
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data()
                         << " (" << physical_device_properties_12.driverName.data()
                         << ") is not suitable because it's missing physical device extensions: "
                         << device_extensions.error();
      continue;
    }

    logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data()
                       << " (" << physical_device_properties_12.driverName.data()
                       << ") has got all required physical device extensions: " << *device_extensions;

    auto const unavailable_features_indices
      = ::detail::find_unavailable_feature_indices(potential_physical_device,
                                                   required_physical_device_features);

    if(not unavailable_features_indices.empty())
    {
      if(unavailable_features_indices.size() == 1)
        logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data()
                           << " (" << physical_device_properties_12.driverName.data()
                           << ") is not suitable because it's missing " << unavailable_features_indices.size()
                           << " required physical device feature at index "
                           << unavailable_features_indices.front();
      else
        logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data()
                           << " (" << physical_device_properties_12.driverName.data()
                           << ") is not suitable because it's missing " << unavailable_features_indices.size()
                           << " required physical device features at indices "
                           << unavailable_features_indices;
      continue;
    }

    logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data()
                       << " (" << physical_device_properties_12.driverName.data()
                       << ") has all requires device features";

    auto const queue_family_properties = potential_physical_device->getQueueFamilyProperties();

    std::uint32_t graphics_queue_family_index = 0;
    for(; graphics_queue_family_index < queue_family_properties.size(); graphics_queue_family_index++)
      if(queue_family_properties[graphics_queue_family_index].queueFlags & vk::QueueFlagBits::eGraphics)
        break;

    if(graphics_queue_family_index == queue_family_properties.size())
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data()
                         << " (" << physical_device_properties_12.driverName.data()
                         << ") is not suitable because it's missing the graphics queue family";
      continue;
    }

    std::uint32_t present_queue_family_index = 0;
    for(; present_queue_family_index < queue_family_properties.size(); present_queue_family_index++)
      if(potential_physical_device->getSurfaceSupportKHR(present_queue_family_index, *surface))
        break;

    if(present_queue_family_index == queue_family_properties.size())
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data()
                         << " (" << physical_device_properties_12.driverName.data()
                         << ") is not suitable because it's missing the present queue family";
      continue;
    }

    if(present_queue_family_index != graphics_queue_family_index)
    {
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data()
                         << " (" << physical_device_properties_12.driverName.data()
                         << ") is not suitable because the present queue family index: "
                         << present_queue_family_index
                         << " differs from graphics queue family index: " << graphics_queue_family_index;
      continue;
    }

    logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data()
                       << " (" << physical_device_properties_12.driverName.data()
                       << ") has present queue family at index " << present_queue_family_index;

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

    graphics_and_present_queue_family_index = graphics_queue_family_index;
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
        .setQueueFamilyIndex(graphics_and_present_queue_family_index)
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
    = vk::DeviceQueueInfo2{}.setQueueFamilyIndex(graphics_and_present_queue_family_index).setQueueIndex(0);

  detail::emplace_data(device, physical_device);

  graphics_and_present_queue
    = detail::make_shared_with_data<vk::raii::Queue const>(*device, graphics_and_present_queue_create_info);

  detail::emplace_data(graphics_and_present_queue, device);
}