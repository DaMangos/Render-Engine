#include "find_device.hpp"

#include "../make_shared_with_deleter_data.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"
#include "vulkan/vulkan_to_string.hpp"

#include <logging/logging.hpp>
#include <logging/serialize.hpp>

#include <algorithm>
#include <cstddef>
#include <expected>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

namespace
{
[[nodiscard]]
static std::expected<std::vector<char const *>, std::vector<char const *>> get_required_device_extensions(
  vk::raii::PhysicalDevice const & physical_device)
{
  auto const is_extension_available =
    [properties = physical_device.enumerateDeviceExtensionProperties()](std::string_view const extension)
  {
    return std::ranges::any_of(properties, [&](auto const & property) { return property.extensionName == extension; });
  };

  auto required_device_extensions = std::vector{vk::KHRSwapchainExtensionName};

  auto unavailable_device_extensions =
    std::vector{std::from_range, required_device_extensions | std::views::filter(std::not_fn(is_extension_available))};

  if(not unavailable_device_extensions.empty())
    return std::unexpected{unavailable_device_extensions};

  if(is_extension_available(vk::KHRPortabilitySubsetExtensionName))
    required_device_extensions.emplace_back(vk::KHRPortabilitySubsetExtensionName);

  return required_device_extensions;
}

[[nodiscard]]
static std::optional<std::uint32_t> find_graphics_queue_index(vk::raii::PhysicalDevice const & physical_device)
{
  auto const properties = physical_device.getQueueFamilyProperties();

  std::uint32_t graphics_queue_index = 0;
  for(; graphics_queue_index < static_cast<std::uint32_t>(properties.size()); graphics_queue_index++)
    if(static_cast<bool>(properties[graphics_queue_index].queueFlags & vk::QueueFlagBits::eGraphics))
      return graphics_queue_index;

  return std::nullopt;
}

[[nodiscard]]
static std::optional<std::uint32_t> find_present_queue_index(vk::raii::PhysicalDevice const & physical_device,
                                                             vk::raii::SurfaceKHR const &     surface)
{
  auto const properties = physical_device.getQueueFamilyProperties();

  std::uint32_t present_queue_index = 0;
  for(; present_queue_index < static_cast<std::uint32_t>(properties.size()); present_queue_index++)
    if(physical_device.getSurfaceSupportKHR(present_queue_index, *surface))
      return present_queue_index;

  return std::nullopt;
}

template <typename... Features>
[[nodiscard]]
std::vector<std::size_t> find_unavailable_feature_indices(
  vk::StructureChain<vk::PhysicalDeviceFeatures2, Features...> const & requested_features,
  vk::StructureChain<vk::PhysicalDeviceFeatures2, Features...> const & supported_features)
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

  auto const requested_bits   = to_bitset(std::apply(flatten, requested_features));
  auto const supported_bits   = to_bitset(std::apply(flatten, supported_features));
  auto const unavailable_bits = requested_bits & ~supported_bits;

  std::vector<std::size_t> unavailable_features_indices;

  unavailable_features_indices.reserve(unavailable_bits.count());

  for(std::size_t unavailable_index = 0, requested_index = 0; unavailable_index < unavailable_bits.size(); ++unavailable_index)
  {
    if(unavailable_bits[unavailable_index])
      unavailable_features_indices.emplace_back(requested_index);

    if(requested_bits[unavailable_index])
      requested_index++;
  }

  return unavailable_features_indices;
}

struct swapchain_data_less
{
    [[nodiscard]]
    constexpr bool operator()(
      std::shared_ptr<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D> const> const & lhs,
      std::shared_ptr<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D> const> const & rhs) const noexcept
    {
      constexpr auto score =
        [](std::shared_ptr<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D> const> const &
             default_swapchain_data)
      {
        if(not default_swapchain_data)
          return 0;

        int score = 0;

        if(std::get<0>(*default_swapchain_data).imageFormat == vk::Format::eB8G8R8A8Srgb)
          score += 1;

        if(std::get<0>(*default_swapchain_data).imageColorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
          score += 1;

        if(std::get<0>(*default_swapchain_data).presentMode == vk::PresentModeKHR::eMailbox)
          score += 3;

        return score;
      };

      return score(lhs) < score(rhs);
    }
};
}

graphics::detail::vulkan::find_device_return_type graphics::detail::vulkan::find_device(
  std::shared_ptr<vk::raii::Instance const> const &   instance,
  std::shared_ptr<vk::raii::SurfaceKHR const> const & surface)
{
  using namespace logging::serialize;

  auto const required_physical_device_features = vk::StructureChain{
    vk::PhysicalDeviceFeatures2{},                       //
    vk::PhysicalDeviceVulkan11Features{}                 //
      .setShaderDrawParameters(true),                    //
    vk::PhysicalDeviceVulkan13Features{}                 //
      .setDynamicRendering(true),                        //
    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT{}  //
      .setExtendedDynamicState(true)                     //
  };

  auto const make_physical_devices = [&](vk::raii::PhysicalDevice const & physical_device)
  {
    return make_shared_with_deleter_data<vk::raii::PhysicalDevice const>(instance, physical_device);
  };

  auto const is_physical_device_suitable = [&](std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device)
  {
    return [&]<class... Features>(vk::StructureChain<Features...> const & required_physical_device_features)
    {
      auto const physical_device_properties = physical_device->getProperties();

      if(physical_device_properties.apiVersion < vk::ApiVersion14)
      {
        logging::warning() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                           << " is not suitable because does not support vulkan api version 1.4";
        return false;
      }

      auto device_extensions = get_required_device_extensions(*physical_device);

      if(not device_extensions)
      {
        logging::warning() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                           << " is not suitable because it's missing physical device extensions: " << device_extensions.error();
        return false;
      }

      logging::verbose() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                         << " has got all required physical device extensions: " << *device_extensions;

      auto const unavailable_features_indices =
        find_unavailable_feature_indices(required_physical_device_features, physical_device->getFeatures2<Features...>());

      if(not unavailable_features_indices.empty())
      {
        if(unavailable_features_indices.size() == 1)
          logging::warning() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                             << " is not suitable because it's missing " << unavailable_features_indices.size()
                             << " required physical device feature at index " << unavailable_features_indices.front();
        else
          logging::warning() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                             << " is not suitable because it's missing " << unavailable_features_indices.size()
                             << " required physical device features at indices " << unavailable_features_indices;
        return false;
      }

      logging::verbose() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                         << " has all requires device features";

      auto const graphics_queue_index = find_graphics_queue_index(*physical_device);

      if(not graphics_queue_index)
      {
        logging::warning() << physical_device_properties.deviceName
                           << " is not suitable because it's missing the graphics queue family";
        return false;
      }

      logging::verbose() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                         << " has graphics queue family at index " << *graphics_queue_index;

      auto const present_queue_index = find_present_queue_index(*physical_device, *surface);

      if(not present_queue_index)
      {
        logging::warning() << physical_device_properties.deviceName
                           << " is not suitable because it's missing the present queue family";
        return false;
      }

      logging::verbose() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                         << " has present queue family at index " << *present_queue_index;
      return true;
    }(required_physical_device_features);
  };

  auto const make_default_swapchain_data = [&](std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device)
    -> std::pair<std::shared_ptr<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D> const>,
                 std::shared_ptr<vk::raii::PhysicalDevice const>>
  {
    auto const physical_device_properties = physical_device->getProperties();

    auto const surface_capabilities = physical_device->getSurfaceCapabilitiesKHR(*surface);

    auto const min_image_count = surface_capabilities.maxImageCount == 0
                                 ? std::max(3u, surface_capabilities.minImageCount)
                                 : std::clamp(3u, surface_capabilities.minImageCount, surface_capabilities.maxImageCount);

    auto const swapchain_data =
      graphics::detail::make_shared_with_deleter_data<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D>>(  //
        surface,                                                                                                            //
        std::make_tuple(vk::SwapchainCreateInfoKHR{}                                                                        //
                          .setSurface(*surface)                                                                             //
                          .setMinImageCount(min_image_count)                                                                //
                          .setImageArrayLayers(1)                                                                           //
                          .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)                                          //
                          .setImageSharingMode(vk::SharingMode::eExclusive)                                                 //
                          .setPreTransform(surface_capabilities.currentTransform)                                           //
                          .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)                                        //
                          .setClipped(true),
                        surface_capabilities.minImageExtent,
                        surface_capabilities.maxImageExtent));

    auto const surface_formats = physical_device->getSurfaceFormatsKHR(*surface);

    if(std::ranges::contains(surface_formats,
                             vk::SurfaceFormatKHR{vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}))
    {
      logging::verbose() << "physical device "                                                            //
                         << std::quoted(physical_device_properties.deviceName.data()) << " has format "   //
                         << std::quoted(vk::to_string(vk::Format::eB8G8R8A8Srgb)) << " and color space "  //
                         << std::quoted(vk::to_string(vk::ColorSpaceKHR::eSrgbNonlinear));

      std::get<0>(*swapchain_data).setImageFormat(vk::Format::eB8G8R8A8Srgb);
      std::get<0>(*swapchain_data).setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear);
    }
    else if(not surface_formats.empty())
    {
      logging::verbose() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                         << " is missing format "                                                                      //
                         << std::quoted(vk::to_string(vk::Format::eB8G8R8A8Srgb)) << " and color space "               //
                         << std::quoted(vk::to_string(vk::ColorSpaceKHR::eSrgbNonlinear)) << " defaulting to format "  //
                         << std::quoted(vk::to_string(surface_formats.front().format)) << " and color space "          //
                         << std::quoted(vk::to_string(surface_formats.front().colorSpace));

      std::get<0>(*swapchain_data).setImageFormat(surface_formats.front().format);
      std::get<0>(*swapchain_data).setImageColorSpace(surface_formats.front().colorSpace);
    }
    else
    {
      logging::warning() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                         << " is not suitable because it's missing surface formats";

      return std::make_pair(nullptr, physical_device);
    }

    auto const surface_present_modes = physical_device->getSurfacePresentModesKHR(*surface);

    if(std::ranges::contains(surface_present_modes, vk::PresentModeKHR::eMailbox))
    {
      logging::verbose() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                         << " has present mode " << std::quoted(vk::to_string(vk::PresentModeKHR::eMailbox));

      std::get<0>(*swapchain_data).setPresentMode(vk::PresentModeKHR::eMailbox);
    }
    else if(std::ranges::contains(surface_present_modes, vk::PresentModeKHR::eFifo))
    {
      logging::verbose() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                         << " is missing present mode " << std::quoted(vk::to_string(vk::PresentModeKHR::eMailbox))
                         << " defaulting to " << std::quoted(vk::to_string(vk::PresentModeKHR::eFifo));

      std::get<0>(*swapchain_data).setPresentMode(vk::PresentModeKHR::eFifo);
    }
    else
    {
      logging::warning() << "physical device " << std::quoted(physical_device_properties.deviceName.data())
                         << " is not suitable because it's missing present mode "
                         << std::quoted(vk::to_string(vk::PresentModeKHR::eFifo));

      return std::make_pair(nullptr, physical_device);
    }

    return std::make_pair(swapchain_data, physical_device);
  };

  auto const suitable_physical_devices =
    std::map<std::shared_ptr<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D> const>,
             std::shared_ptr<vk::raii::PhysicalDevice const>,
             swapchain_data_less>{std::from_range,
                                  instance->enumeratePhysicalDevices() |               //
                                    std::views::transform(make_physical_devices) |     //
                                    std::views::filter(is_physical_device_suitable) |  //
                                    std::views::transform(make_default_swapchain_data)};

  if(suitable_physical_devices.empty())
    throw std::runtime_error("there are no suitable physical devices");

  auto const & [default_swapchain_data, physical_device] = *std::prev(suitable_physical_devices.end());

  if(not default_swapchain_data)
    throw std::runtime_error("there are no suitable physical devices");

  auto const graphics_queue_index = find_graphics_queue_index(*physical_device);

  auto const present_queue_index = find_present_queue_index(*physical_device, *surface);

  constexpr auto queue_priority = std::array{0.5f};

  auto device_queue_create_infos = std::array{
    vk::DeviceQueueCreateInfo{}                    //
      .setQueueFamilyIndex(*graphics_queue_index)  //
      .setQueueCount(1)                            //
      .setQueuePriorities(queue_priority),         //
    vk::DeviceQueueCreateInfo{}                    //
      .setQueueFamilyIndex(*present_queue_index)   //
      .setQueueCount(1)                            //
      .setQueuePriorities(queue_priority)          //
  };

  auto const queue_family_index_compare = [](vk::DeviceQueueCreateInfo const & lhs, vk::DeviceQueueCreateInfo const & rhs)
  {
    return lhs.queueFamilyIndex == rhs.queueFamilyIndex;
  };

  auto const unique_device_queue_create_infos =
    std::vector{std::from_range, std::ranges::unique(device_queue_create_infos, queue_family_index_compare)};

  auto const required_device_extensions = get_required_device_extensions(*physical_device);

  auto const device_create_info = vk::DeviceCreateInfo{}                                    //
                                    .setPNext(&required_physical_device_features.get())     //
                                    .setQueueCreateInfos(unique_device_queue_create_infos)  //
                                    .setPEnabledExtensionNames(*required_device_extensions);

  auto const device = make_shared_with_deleter_data<vk::raii::Device const>(  //
    physical_device,                                                          //
    *physical_device,                                                         //
    device_create_info);

  auto const create_queue = [&](vk::DeviceQueueCreateInfo const & device_queue_create_info)
  {
    auto const queue_create_info = vk::DeviceQueueInfo2{}                                             //
                                     .setQueueFamilyIndex(device_queue_create_info.queueFamilyIndex)  //
                                     .setQueueIndex(0);

    return std::make_pair(                                   //
      queue_create_info.queueFamilyIndex,                    //
      make_shared_with_deleter_data<vk::raii::Queue const>(  //
        device,                                              //
        *device,                                             //
        queue_create_info));
  };

  auto const queues =
    std::unordered_map{std::from_range, unique_device_queue_create_infos | std::views::transform(create_queue)};

  return {
    .physical_device        = physical_device,
    .device                 = device,
    .graphics_queue         = queues.find(*graphics_queue_index)->second,
    .present_queue          = queues.find(*present_queue_index)->second,
    .default_swapchain_data = default_swapchain_data,
  };
}