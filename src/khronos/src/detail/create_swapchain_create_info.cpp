#include "create_swapchain_create_info.hpp"

#include "device_settings.hpp"
#include "find_queue_index.hpp"
#include "make_shared_with_data.hpp"
#include "vulkan/vulkan_to_string.hpp"

#include <logging/logging.hpp>
#include <logging/serialize.hpp>

#include <iomanip>

namespace
{
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
}  // namespace

std::shared_ptr<vk::SwapchainCreateInfoKHR> khronos::detail::create_swapchain_create_info(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
  std::shared_ptr<vk::raii::SurfaceKHR const> const &     surface)
{
  using namespace logging::serialize;

  auto const & [physical_device_properties,
                physical_device_properties_12] = physical_device
                                                   ->getProperties2<vk::PhysicalDeviceProperties2,
                                                                    vk::PhysicalDeviceVulkan12Properties>();

  if(physical_device_properties.properties.apiVersion < vk::ApiVersion13)
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data()
                       << ") is not suitable because does not support vulkan api version 1.3";
    return nullptr;
  }

  auto device_extensions = get_required_device_extensions(physical_device);

  if(not device_extensions)
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data()
                       << ") is not suitable because it's missing physical device extensions: "
                       << device_extensions.error();
    return nullptr;
  }

  logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                     << physical_device_properties_12.driverName.data()
                     << ") has got all required physical device extensions: " << *device_extensions;

  auto const unavailable_features_indices = find_unavailable_feature_indices(physical_device,
                                                                             required_physical_device_features);

  if(not unavailable_features_indices.empty())
  {
    if(unavailable_features_indices.size() == 1)
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                         << physical_device_properties_12.driverName.data() << ") is not suitable because it's missing "
                         << unavailable_features_indices.size() << " required physical device feature at index "
                         << unavailable_features_indices.front();
    else
      logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                         << physical_device_properties_12.driverName.data() << ") is not suitable because it's missing "
                         << unavailable_features_indices.size() << " required physical device features at indices "
                         << unavailable_features_indices;
    return nullptr;
  }

  logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                     << physical_device_properties_12.driverName.data() << ") has all requires device features";

  auto const graphics_queue_index = find_graphics_queue_index(physical_device);

  if(not graphics_queue_index)
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data()
                       << ") is not suitable because it's missing the graphics queue family";
    return nullptr;
  }

  logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                     << physical_device_properties_12.driverName.data() << ") has graphics queue family at index "
                     << *graphics_queue_index;

  auto const present_queue_index = find_present_queue_index(physical_device, surface);

  if(not present_queue_index)
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data()
                       << ") is not suitable because it's missing the present queue family";
    return nullptr;
  }

  logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                     << physical_device_properties_12.driverName.data() << ") has present queue family at index "
                     << *present_queue_index;

  auto const surface_capabilities = physical_device->getSurfaceCapabilitiesKHR(*surface);

  auto const min_image_count = surface_capabilities.maxImageCount == 0
                               ? std::max(3u, surface_capabilities.minImageCount)
                               : std::clamp(3u, surface_capabilities.minImageCount, surface_capabilities.maxImageCount);

  auto const queue_family_indices = std::make_shared<std::vector<std::uint32_t>>();

  if(graphics_queue_index)
    queue_family_indices->emplace_back(*graphics_queue_index);

  if(present_queue_index)
    queue_family_indices->emplace_back(*present_queue_index);

  auto const swapchain_create_info = make_shared_with_data<
    vk::SwapchainCreateInfoKHR>(std::make_tuple(physical_device, surface, queue_family_indices),
                                vk::SwapchainCreateInfoKHR{}
                                  .setSurface(*surface)
                                  .setMinImageCount(min_image_count)
                                  .setImageArrayLayers(1)
                                  .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
                                  .setPreTransform(surface_capabilities.currentTransform)
                                  .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
                                  .setClipped(vk::True));

  if(queue_family_indices->size() == 2)
  {
    swapchain_create_info->setQueueFamilyIndices(*queue_family_indices);
    swapchain_create_info->setImageSharingMode(vk::SharingMode::eExclusive);
  }
  else
    swapchain_create_info->setImageSharingMode(vk::SharingMode::eConcurrent);

  auto const surface_formats = physical_device->getSurfaceFormatsKHR(*surface);

  if(std::ranges::contains(surface_formats,
                           vk::SurfaceFormatKHR{vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}))
  {
    logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data() << ") has format "
                       << std::quoted(vk::to_string(vk::Format::eB8G8R8A8Srgb)) << " and color space "
                       << std::quoted(vk::to_string(vk::ColorSpaceKHR::eSrgbNonlinear));

    swapchain_create_info->setImageFormat(vk::Format::eB8G8R8A8Srgb);
    swapchain_create_info->setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear);
  }
  else if(not surface_formats.empty())
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data() << ") is missing format "
                       << std::quoted(vk::to_string(vk::Format::eB8G8R8A8Srgb)) << " and color space "
                       << std::quoted(vk::to_string(vk::ColorSpaceKHR::eSrgbNonlinear)) << " defaulting to format "
                       << std::quoted(vk::to_string(surface_formats.front().format)) << " and color space "
                       << std::quoted(vk::to_string(surface_formats.front().colorSpace));

    swapchain_create_info->setImageFormat(surface_formats.front().format);
    swapchain_create_info->setImageColorSpace(surface_formats.front().colorSpace);
  }
  else
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data()
                       << ") is not suitable because it's missing surface formats";
    return nullptr;
  }

  auto const surface_present_modes = physical_device->getSurfacePresentModesKHR(*surface);

  if(std::ranges::contains(surface_present_modes, vk::PresentModeKHR::eMailbox))
  {
    logging::verbose() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data() << ") has present mode "
                       << std::quoted(vk::to_string(vk::PresentModeKHR::eMailbox));

    swapchain_create_info->setPresentMode(vk::PresentModeKHR::eMailbox);
  }
  else if(std::ranges::contains(surface_present_modes, vk::PresentModeKHR::eFifo))
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data() << ") is missing present mode "
                       << std::quoted(vk::to_string(vk::PresentModeKHR::eMailbox)) << " defaulting to "
                       << std::quoted(vk::to_string(vk::PresentModeKHR::eFifo));

    swapchain_create_info->setPresentMode(vk::PresentModeKHR::eFifo);
  }
  else
  {
    logging::warning() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                       << physical_device_properties_12.driverName.data()
                       << ") is not suitable because it's missing present mode "
                       << std::quoted(vk::to_string(vk::PresentModeKHR::eFifo));
    return nullptr;
  }

  return swapchain_create_info;
};