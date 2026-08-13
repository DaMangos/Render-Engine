#pragma once

#include <glfw/window.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <memory>

namespace khronos
{
class present_window : public glfw::window
{
  private:
    friend class library;

    present_window(glfw::window && window, std::shared_ptr<vk::raii::Instance const> const & instance);

  protected:
    std::shared_ptr<vk::raii::SurfaceKHR const> surface;
};
}