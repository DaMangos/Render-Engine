#pragma once

#include <glfw/window.hpp>

namespace khronos
{
class present_window : public glfw::window
{
  public:
    present_window(glfw::window && window, class library const & library);

    present_window(present_window &&) noexcept = default;

    present_window(present_window const &) noexcept = delete;

    present_window & operator=(present_window const &) noexcept = delete;

    present_window & operator=(present_window &&) noexcept = default;

    ~present_window() noexcept override;

  protected:
    friend class graphical_device;

    std::unique_ptr<struct present_window_impl> self;
};
}