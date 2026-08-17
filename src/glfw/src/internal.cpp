#include "internal.hpp"

#include <glfw/window.hpp>

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

  if(self and self->when_window_moved)
    self->when_window_moved(*self, {.x = xpos, .y = ypos});
}

void glfw::internal::window_size_callback(GLFWwindow * window, int width, int height)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_resized)
    self->when_window_resized(*self, {.width = width, .height = height});
}

void glfw::internal::window_close_callback(GLFWwindow * window)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_closed)
    self->when_window_closed(*self);
}

void glfw::internal::window_refresh_callback(GLFWwindow * window)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_refreshed)
    self->when_window_refreshed(*self);
}

void glfw::internal::window_focus_callback(GLFWwindow * window, int focused)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_focused and focused == VK_TRUE)
    self->when_window_focused(*self);

  if(self and self->when_window_unfocused and focused == VK_FALSE)
    self->when_window_unfocused(*self);
}

void glfw::internal::window_iconify_callback(GLFWwindow * window, int iconified)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_minimized and iconified == VK_TRUE)
    self->when_window_minimized(*self);

  if(self and self->when_window_unminimized and iconified == VK_FALSE)
    self->when_window_unminimized(*self);
}

void glfw::internal::window_maximize_callback(GLFWwindow * window, int maximized)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_maximized and maximized == VK_TRUE)
    self->when_window_maximized(*self);

  if(self and self->when_window_unmaximized and maximized == VK_FALSE)
    self->when_window_unmaximized(*self);
}

void glfw::internal::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_framebuffer_resized)
    self->when_framebuffer_resized(*self, {.width = width, .height = height});
}

void glfw::internal::window_content_scale_callback(GLFWwindow * window, float xscale, float yscale)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_content_scaled)
    self->when_window_content_scaled(*self, {.x = xscale, .y = yscale});
}

void glfw::internal::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_key_pressed)
    self->when_key_pressed(*self, glfw::key{key}, scancode, glfw::action{action}, glfw::modifier{mods});
}

void glfw::internal::char_callback(GLFWwindow * window, unsigned int codepoint)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_unicode_char_typed)
    self->when_unicode_char_typed(*self, static_cast<char32_t>(codepoint));
}

void glfw::internal::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_mouse_button_pressed)
    self->when_mouse_button_pressed(*self, glfw::mouse_button{button}, glfw::action{action}, glfw::modifier{mods});
}

void glfw::internal::cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_cursor_moved)
    self->when_cursor_moved(*self, {.x = xpos, .y = ypos});
}

void glfw::internal::cursor_enter_callback(GLFWwindow * window, int entered)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_cursor_entered and entered == VK_TRUE)
    self->when_cursor_entered(*self);

  if(self and self->when_cursor_exited and entered == VK_FALSE)
    self->when_cursor_exited(*self);
}

void glfw::internal::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_mouse_scrolled)
    self->when_mouse_scrolled(*self, {.x = xoffset, .y = yoffset});
}

void glfw::internal::drop_callback(GLFWwindow * window, int path_count, char const ** paths)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_file_dropped)
  {
    std::vector<std::filesystem::path> files;

    files.reserve(static_cast<std::size_t>(path_count));

    for(auto const & path : std::span{paths, std::ranges::next(paths, path_count)})
      files.emplace_back(path);

    self->when_file_dropped(*self, files);
  }
}