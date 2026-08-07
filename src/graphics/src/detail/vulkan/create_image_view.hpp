#pragma once

#include "vulkan/vulkan_raii.hpp"

#include <vulkan/vulkan_raii.hpp>

#include <memory>

namespace graphics::detail::vulkan
{
[[nodiscard]]
std::vector<std::shared_ptr<vk::raii::ImageView const>> create_image_view(
  std::shared_ptr<vk::raii::Device const> const &                                                   device,
  std::shared_ptr<vk::raii::SwapchainKHR const> const &                                             swapchain,
  std::shared_ptr<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D> const> const & swapchain_data);
}