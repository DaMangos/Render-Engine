#include "find_queue_index.hpp"

std::optional<std::uint32_t> vulkan_1::detail::find_graphics_queue_index(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device)
{
  auto const properties = physical_device->getQueueFamilyProperties();

  std::uint32_t graphics_queue_index = 0;
  for(; graphics_queue_index < static_cast<std::uint32_t>(properties.size()); graphics_queue_index++)
    if(static_cast<bool>(properties[graphics_queue_index].queueFlags & vk::QueueFlagBits::eGraphics))
      return graphics_queue_index;

  return std::nullopt;
}

std::optional<std::uint32_t> vulkan_1::detail::find_present_queue_index(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
  std::shared_ptr<vk::raii::SurfaceKHR const> const &     surface)
{
  auto const properties = physical_device->getQueueFamilyProperties();

  std::uint32_t present_queue_index = 0;
  for(; present_queue_index < static_cast<std::uint32_t>(properties.size()); present_queue_index++)
    if(physical_device->getSurfaceSupportKHR(present_queue_index, *surface))
      return present_queue_index;

  return std::nullopt;
}