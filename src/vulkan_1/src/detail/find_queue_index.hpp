#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <memory>
#include <optional>

namespace vulkan_1::detail
{
[[nodiscard]]
std::optional<std::uint32_t> find_graphics_queue_index(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device);

[[nodiscard]]
std::optional<std::uint32_t> find_present_queue_index(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
  std::shared_ptr<vk::raii::SurfaceKHR const> const &     surface);
}