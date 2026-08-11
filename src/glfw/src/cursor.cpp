#include <glfw_3/cursor.hpp>

glfw_3::cursor::cursor(std::unique_ptr<GLFWcursor, void (*)(GLFWcursor *)> && ptr) noexcept
: ptr(std::move(ptr))
{
}