#pragma once

#include <GLFW/glfw3.h>

#include <memory>

namespace graphics::detail::glfw
{
[[nodiscard]]
std::shared_ptr<GLFWwindow> create_window(int width, int height, char const * title, GLFWmonitor * monitor, GLFWwindow * share);
}