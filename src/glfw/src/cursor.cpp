#include <glfw/cursor.hpp>

glfw::cursor::cursor(std::unique_ptr<GLFWcursor, void (*)(GLFWcursor *)> && ptr) noexcept
: ptr(std::move(ptr))
{
}