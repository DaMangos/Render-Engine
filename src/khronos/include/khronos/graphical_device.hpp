#pragma once

#include <memory>

namespace khronos
{
class graphical_device
{
  public:
    explicit graphical_device(class present_window const & present_window);

    graphical_device(graphical_device &&) noexcept = default;

    graphical_device(graphical_device const &) noexcept = delete;

    graphical_device & operator=(graphical_device const &) noexcept = delete;

    graphical_device & operator=(graphical_device &&) noexcept = default;

    ~graphical_device() noexcept;

  private:
    friend class graphics_pipeline;
    friend class transfer_command;
    friend class draw_command;
    friend class render_window;
    friend class staging_buffer;
    friend class transfer_buffer;

    std::unique_ptr<struct graphical_device_impl> self;
};
}