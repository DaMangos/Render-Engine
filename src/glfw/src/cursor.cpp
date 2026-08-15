#include <glfw/cursor.hpp>

#include <cassert>

glfw::cursor::cursor(std::unique_ptr<GLFWcursor, void (*)(GLFWcursor *)> && new_ptr) noexcept
: ptr(std::move(new_ptr))
{
  assert(ptr);
}