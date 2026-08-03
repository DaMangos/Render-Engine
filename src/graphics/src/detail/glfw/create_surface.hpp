#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>

#include <memory>

namespace graphics::detail::glfw
{
[[nodiscard]]
std::shared_ptr<vk::raii::SurfaceKHR const> create_surface(std::shared_ptr<vk::raii::Instance const> const & instance,
                                                           std::shared_ptr<GLFWwindow> const &               window);
}