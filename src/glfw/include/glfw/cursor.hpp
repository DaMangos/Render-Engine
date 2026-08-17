#pragma once

#include <glfw/fwd.hpp>

#include <memory>

namespace glfw
{
class cursor
{
    [[nodiscard]]
    std::strong_ordering operator<=>(cursor const & other) const noexcept = default;

  private:
    friend class window;
    friend class library;

    explicit cursor(std::unique_ptr<GLFWcursor, void (*)(GLFWcursor *)> && new_ptr) noexcept;

    std::unique_ptr<GLFWcursor, void (*)(GLFWcursor *)> ptr;
};
}
