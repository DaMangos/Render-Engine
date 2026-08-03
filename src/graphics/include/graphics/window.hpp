#pragma once

#include <graphics/pixel.hpp>

#include <memory>
#include <ostream>
#include <string>

namespace graphics
{
class window
{
  public:
    window(pixel const          width,
           pixel const          hight,
           std::string const &  title,
           std::ostream * const vk_verbose_out,
           std::ostream * const vk_info_out,
           std::ostream * const vk_warning_out,
           std::ostream * const vk_error_out);

    [[nodiscard]]
    bool is_valueless_after_move() const noexcept;

    [[nodiscard]]
    bool should_close() const;

    void poll_events() const;

    void close() noexcept;

  private:
    std::unique_ptr<void, void (*)(void *)> window_impl;
};
}