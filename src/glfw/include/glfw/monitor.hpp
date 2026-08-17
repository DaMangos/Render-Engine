#pragma once

#include <glfw/fwd.hpp>

#include <compare>
#include <string_view>
#include <vector>

namespace glfw
{
class monitor
{
  public:
    monitor(monitor &&) noexcept = default;

    monitor(monitor const &) noexcept = delete;

    monitor & operator=(monitor &&) noexcept = default;

    monitor & operator=(monitor const &) noexcept = delete;

    ~monitor() = default;

    [[nodiscard]]
    std::strong_ordering operator<=>(monitor const & other) const noexcept;

    [[nodiscard]]
    coordinates<int, 2> get_pos() const;

    [[nodiscard]]
    workarea get_workarea() const;

    [[nodiscard]]
    dimensions<int, 2> get_physical_size_millimeters() const;

    [[nodiscard]]
    coordinates<float, 2> get_content_scale() const;

    [[nodiscard]]
    std::string_view get_name() const;

    [[nodiscard]]
    std::vector<vidmode> get_video_modes() const;

    [[nodiscard]]
    vidmode get_current_video_mode() const;

  private:
    friend struct internal;

    friend window;
    friend library;

    explicit monitor(GLFWmonitor * new_ptr) noexcept;

    GLFWmonitor * ptr = nullptr;
};
}
