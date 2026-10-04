#pragma once

#include "graphics/graphics_pipeline.hpp"

#include <graphics/present_window.hpp>

namespace graphics
{
class render_window : public present_window
{
  public:
    render_window(present_window && present_window, class graphical_device const & graphical_device);

    render_window(render_window &&) noexcept = default;

    render_window(render_window const &) noexcept = delete;

    render_window & operator=(render_window const &) noexcept = delete;

    render_window & operator=(render_window &&) noexcept = default;

    ~render_window() noexcept override;

  protected:
    void when_framebuffer_resized(glfw::dimensions<int, 2> const & size) override;

  private:
    friend graphics_pipeline;

    std::unique_ptr<struct render_window_impl> self;
};
}