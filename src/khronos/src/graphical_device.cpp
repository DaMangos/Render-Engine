#include "detail/create_swapchain_create_info.hpp"
#include "detail/device_settings.hpp"
#include "detail/find_queue_index.hpp"
#include "detail/make_shared_with_data.hpp"

#include <khronos/graphical_device.hpp>
#include <khronos/library.hpp>
#include <khronos/present_window.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>
#include <logging/serialize.hpp>

#include <algorithm>
#include <cassert>
#include <expected>
#include <memory>
#include <ranges>
#include <string_view>
#include <vector>

namespace
{
[[nodiscard]]
static int score(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device) noexcept
{
  if(not physical_device)
    return 0;

  auto const is_extension_available
    = [properties = physical_device->enumerateDeviceExtensionProperties()](std::string_view const extension)
  {
    return std::ranges::any_of(properties, [&](auto const & property) { return property.extensionName == extension; });
  };

  return is_extension_available(vk::KHRPortabilitySubsetExtensionName) ? 0 : 1;
}

[[nodiscard]]
static int score(std::shared_ptr<vk::SwapchainCreateInfoKHR const> const & swapchain_create_info) noexcept
{
  if(not swapchain_create_info)
    return 0;

  int score = 0;

  if(swapchain_create_info->imageFormat == vk::Format::eB8G8R8A8Srgb)
    score += 1;

  if(swapchain_create_info->imageColorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
    score += 1;

  if(swapchain_create_info->presentMode == vk::PresentModeKHR::eMailbox)
    score += 3;

  return score;
}
}

khronos::render_window khronos::graphical_device::create_render_window(present_window && window) const
{
  return {std::move(window),
          physical_device,
          device,
          default_swapchain_create_info,
          min_image_extent,
          max_image_extent};
}

khronos::graphical_device::graphical_device(std::shared_ptr<vk::raii::Instance const>           instance,
                                            std::shared_ptr<vk::raii::SurfaceKHR const> const & surface)
{
  using namespace logging::serialize;

  auto const make_shared_physical_device = [&](auto const & physical_device)
  {
    return detail::make_shared_with_data<vk::raii::PhysicalDevice const>(instance, physical_device);
  };

  auto const physical_devices
    = instance->enumeratePhysicalDevices() | std::views::transform(make_shared_physical_device);

  logging::verbose() << "there are " << physical_devices.size() << " physical devices available";

  for(auto const & potential_physical_device : physical_devices)
  {
    auto const potential_swapchain_create_info
      = detail::create_swapchain_create_info(potential_physical_device, surface);

    if((score(potential_swapchain_create_info) + score(potential_physical_device))
       > score(default_swapchain_create_info) + score(physical_device))
    {
      physical_device               = potential_physical_device;
      default_swapchain_create_info = potential_swapchain_create_info;

      auto const surface_capabilities = physical_device->getSurfaceCapabilitiesKHR(*surface);

      min_image_extent = std::make_shared<vk::Extent2D const>(surface_capabilities.minImageExtent);
      max_image_extent = std::make_shared<vk::Extent2D const>(surface_capabilities.maxImageExtent);
    }
  };

  if(not physical_device or not default_swapchain_create_info or not min_image_extent or not max_image_extent)
    throw std::runtime_error("there are no suitable physical devices");

  auto const & [physical_device_properties, physical_device_properties_12]
    = physical_device->getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVulkan12Properties>();

  logging::info() << "physical device: " << physical_device_properties.properties.deviceName.data() << " ("
                  << physical_device_properties_12.driverName.data() << ") is suitable";

  auto const graphics_queue_index = detail::find_graphics_queue_index(physical_device);
  auto const present_queue_index  = detail::find_present_queue_index(physical_device, surface);

  constexpr auto queue_priority = std::array{0.5f};

  auto device_queue_create_infos = std::array{vk::DeviceQueueCreateInfo{}
                                                .setQueueFamilyIndex(*graphics_queue_index)
                                                .setQueueCount(1)
                                                .setQueuePriorities(queue_priority),
                                              vk::DeviceQueueCreateInfo{}
                                                .setQueueFamilyIndex(*present_queue_index)
                                                .setQueueCount(1)
                                                .setQueuePriorities(queue_priority)};

  auto const queue_family_index_compare
    = [](vk::DeviceQueueCreateInfo const & lhs, vk::DeviceQueueCreateInfo const & rhs)
  {
    return lhs.queueFamilyIndex == rhs.queueFamilyIndex;
  };

  auto const unique_device_queue_create_infos
    = std::vector{std::from_range, std::ranges::unique(device_queue_create_infos, queue_family_index_compare)};

  auto const required_device_extensions = detail::get_required_device_extensions(physical_device);

  auto const device_create_info = vk::DeviceCreateInfo{}
                                    .setPNext(&detail::required_physical_device_features.get())
                                    .setQueueCreateInfos(unique_device_queue_create_infos)
                                    .setPEnabledExtensionNames(*required_device_extensions);

  device = detail::make_shared_with_data<vk::raii::Device const>(physical_device, *physical_device, device_create_info);

  auto const create_queue = [&](vk::DeviceQueueCreateInfo const & device_queue_create_info)
  {
    auto const queue_create_info
      = vk::DeviceQueueInfo2{}.setQueueFamilyIndex(device_queue_create_info.queueFamilyIndex).setQueueIndex(0);

    return std::make_pair(queue_create_info.queueFamilyIndex,
                          detail::make_shared_with_data<vk::raii::Queue const>(device, *device, queue_create_info));
  };

  auto const queues
    = std::unordered_map{std::from_range, unique_device_queue_create_infos | std::views::transform(create_queue)};

  graphics_queue = queues.find(*graphics_queue_index)->second;
  present_queue  = queues.find(*present_queue_index)->second;
}