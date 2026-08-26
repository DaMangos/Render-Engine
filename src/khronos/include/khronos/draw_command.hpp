#pragma once

#include <memory>

namespace khronos
{
class draw_command
{
  public:
    draw_command(class graphical_device const & graphical_device,
                 class render_window const &    render_window,
                 std::size_t const              command_buffer_count);

    draw_command(draw_command &&) noexcept = default;

    draw_command(draw_command const &) noexcept = delete;

    draw_command & operator=(draw_command const &) noexcept = delete;

    draw_command & operator=(draw_command &&) noexcept = default;

    ~draw_command() noexcept;

    void operator()(class graphics_pipeline & graphics_pipeline,
                    class transfer_buffer &   transfer_buffer,
                    class render_window &     render_window);

  private:
    std::unique_ptr<struct draw_command_impl> self;
};
}