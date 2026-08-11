#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <memory>

namespace vulkan_1::detail
{
std::shared_ptr<vk::SwapchainCreateInfoKHR> create_swapchain_create_info(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
  std::shared_ptr<vk::raii::SurfaceKHR const> const &     surface);
}