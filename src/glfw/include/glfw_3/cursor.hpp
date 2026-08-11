#pragma once

#include <memory>

typedef struct GLFWcursor GLFWcursor;

namespace glfw_3
{
class cursor
{
    [[nodiscard]]
    std::strong_ordering operator<=>(cursor const & other) const noexcept = default;

  private:
    friend class window;
    friend class library;

    explicit cursor(std::unique_ptr<GLFWcursor, void (*)(GLFWcursor *)> && ptr) noexcept;

    std::unique_ptr<GLFWcursor, void (*)(GLFWcursor *)> ptr;
};
}
