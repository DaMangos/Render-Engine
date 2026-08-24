#include "internal.hpp"

#include <GLFW/glfw3.h>

glfw::library glfw::internal::init_library()
{
  return {};
}

std::set<glfw::monitor>::iterator glfw::internal::try_emplace_monitor(GLFWmonitor * glfw_monitor)
{
  auto found = monitors.find(monitor(glfw_monitor));

  return found == monitors.end() ? monitors.emplace_hint(monitors.end(), monitor(glfw_monitor)) : found;
}

std::set<glfw::monitor> glfw::internal::monitors;

void glfw::internal::window_pos_callback(GLFWwindow * window, int xpos, int ypos)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_window_moved({.x = xpos, .y = ypos});
}

void glfw::internal::window_size_callback(GLFWwindow * window, int width, int height)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_window_resized({.width = width, .height = height});
}

void glfw::internal::window_close_callback(GLFWwindow * window)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_window_closed();
}

void glfw::internal::window_refresh_callback(GLFWwindow * window)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_window_refreshed();
}

void glfw::internal::window_focus_callback(GLFWwindow * window, int focused)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and focused == VK_TRUE)
    self->when_window_focused();

  if(self and focused == VK_FALSE)
    self->when_window_unfocused();
}

void glfw::internal::window_iconify_callback(GLFWwindow * window, int iconified)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and iconified == VK_TRUE)
    self->when_window_minimized();

  if(self and iconified == VK_FALSE)
    self->when_window_unminimized();
}

void glfw::internal::window_maximize_callback(GLFWwindow * window, int maximized)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and maximized == VK_TRUE)
    self->when_window_maximized();

  if(self and maximized == VK_FALSE)
    self->when_window_unmaximized();
}

void glfw::internal::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_framebuffer_resized({.width = width, .height = height});
}

void glfw::internal::window_content_scale_callback(GLFWwindow * window, float xscale, float yscale)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_window_content_scaled({.x = xscale, .y = yscale});
}

void glfw::internal::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_key_pressed(glfw::key{key}, scancode, glfw::action{action}, glfw::modifier{mods});
}

void glfw::internal::char_callback(GLFWwindow * window, unsigned int codepoint)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_unicode_char_typed(static_cast<char32_t>(codepoint));
}

void glfw::internal::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_mouse_button_pressed(glfw::mouse_button{button}, glfw::action{action}, glfw::modifier{mods});
}

void glfw::internal::cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_cursor_moved({.x = xpos, .y = ypos});
}

void glfw::internal::cursor_enter_callback(GLFWwindow * window, int entered)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and entered == VK_TRUE)
    self->when_cursor_entered();

  if(self and entered == VK_FALSE)
    self->when_cursor_exited();
}

void glfw::internal::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
    self->when_mouse_scrolled({.x = xoffset, .y = yoffset});
}

void glfw::internal::drop_callback(GLFWwindow * window, int path_count, char const ** paths)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self)
  {
    std::vector<std::filesystem::path> files;

    files.reserve(static_cast<std::size_t>(path_count));

    for(auto const & path : std::span{paths, std::ranges::next(paths, path_count)})
      files.emplace_back(path);

    self->when_file_dropped(files);
  }
}