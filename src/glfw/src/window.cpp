#include "internal.hpp"

#include <glfw_3/cursor.hpp>
#include <glfw_3/library.hpp>
#include <glfw_3/monitor.hpp>
#include <glfw_3/window.hpp>

#include <GLFW/glfw3.h>

#include <filesystem>
#include <utility>

namespace
{
static void window_pos_callback(GLFWwindow * window, int xpos, int ypos)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_moved)
    self->when_window_moved({.x = xpos, .y = ypos});
}

static void window_size_callback(GLFWwindow * window, int width, int height)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_resized)
    self->when_window_resized({.width = width, .height = height});
}

static void window_close_callback(GLFWwindow * window)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_closed)
    self->when_window_closed();
}

static void window_refresh_callback(GLFWwindow * window)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_refreshed)
    self->when_window_refreshed();
}

static void window_focus_callback(GLFWwindow * window, int focused)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_focused and focused == VK_TRUE)
    self->when_window_focused();

  if(self and self->when_window_unfocused and focused == VK_FALSE)
    self->when_window_unfocused();
}

static void window_iconify_callback(GLFWwindow * window, int iconified)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_minimized and iconified == VK_TRUE)
    self->when_window_minimized();

  if(self and self->when_window_unminimized and iconified == VK_FALSE)
    self->when_window_unminimized();
}

static void window_maximize_callback(GLFWwindow * window, int maximized)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_maximized and maximized == VK_TRUE)
    self->when_window_maximized();

  if(self and self->when_window_unmaximized and maximized == VK_FALSE)
    self->when_window_unmaximized();
}

static void framebuffer_size_callback(GLFWwindow * window, int xpos, int ypos)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_framebuffer_resized)
    self->when_framebuffer_resized({.x = xpos, .y = ypos});
}

static void window_content_scale_callback(GLFWwindow * window, float xscale, float yscale)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_content_scaled)
    self->when_window_content_scaled({.x = xscale, .y = yscale});
}

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_key_pressed)
    self->when_key_pressed(glfw_3::key{key}, scancode, glfw_3::action{action}, glfw_3::modifier{mods});
}

static void char_callback(GLFWwindow * window, unsigned int codepoint)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_unicode_char_typed)
    self->when_unicode_char_typed(static_cast<char32_t>(codepoint));
}

static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_mouse_button_pressed)
    self->when_mouse_button_pressed(glfw_3::mouse_button{button}, glfw_3::action{action}, glfw_3::modifier{mods});
}

static void cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_cursor_moved)
    self->when_cursor_moved({.x = xpos, .y = ypos});
}

static void cursor_enter_callback(GLFWwindow * window, int entered)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_cursor_entered and entered == VK_TRUE)
    self->when_cursor_entered();

  if(self and self->when_cursor_exited and entered == VK_FALSE)
    self->when_cursor_exited();
}

static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_mouse_scrolled)
    self->when_mouse_scrolled({.x = xoffset, .y = yoffset});
}

static void drop_callback(GLFWwindow * window, int path_count, char const ** paths)
{
  auto * const self = static_cast<glfw_3::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_file_dropped)
  {
    std::vector<std::filesystem::path> files;

    files.reserve(static_cast<std::size_t>(path_count));

    for(auto const & path : std::span{paths, std::ranges::next(paths, path_count)})
      files.emplace_back(path);

    self->when_file_dropped(files);
  }
}
}

glfw_3::window::window(window && other) noexcept
: when_window_moved(std::move(other.when_window_moved)),
  when_window_resized(std::move(other.when_window_resized)),
  when_window_closed(std::move(other.when_window_closed)),
  when_window_refreshed(std::move(other.when_window_refreshed)),
  when_window_focused(std::move(other.when_window_focused)),
  when_window_unfocused(std::move(other.when_window_unfocused)),
  when_window_minimized(std::move(other.when_window_minimized)),
  when_window_unminimized(std::move(other.when_window_unminimized)),
  when_window_maximized(std::move(other.when_window_maximized)),
  when_window_unmaximized(std::move(other.when_window_unmaximized)),
  when_framebuffer_resized(std::move(other.when_framebuffer_resized)),
  when_window_content_scaled(std::move(other.when_window_content_scaled)),
  when_key_pressed(std::move(other.when_key_pressed)),
  when_unicode_char_typed(std::move(other.when_unicode_char_typed)),
  when_mouse_button_pressed(std::move(other.when_mouse_button_pressed)),
  when_cursor_moved(std::move(other.when_cursor_moved)),
  when_cursor_entered(std::move(other.when_cursor_entered)),
  when_cursor_exited(std::move(other.when_cursor_exited)),
  when_mouse_scrolled(std::move(other.when_mouse_scrolled)),
  when_file_dropped(std::move(other.when_file_dropped)),
  ptr(std::move(other.ptr))
{
  if(ptr)
    glfwSetWindowUserPointer(ptr.get(), this);
}

glfw_3::window & glfw_3::window::operator=(window && other) noexcept
{
  when_window_moved          = std::move(other.when_window_moved);
  when_window_resized        = std::move(other.when_window_resized);
  when_window_closed         = std::move(other.when_window_closed);
  when_window_refreshed      = std::move(other.when_window_refreshed);
  when_window_focused        = std::move(other.when_window_focused);
  when_window_unfocused      = std::move(other.when_window_unfocused);
  when_window_minimized      = std::move(other.when_window_minimized);
  when_window_unminimized    = std::move(other.when_window_unminimized);
  when_window_maximized      = std::move(other.when_window_maximized);
  when_window_unmaximized    = std::move(other.when_window_unmaximized);
  when_framebuffer_resized   = std::move(other.when_framebuffer_resized);
  when_window_content_scaled = std::move(other.when_window_content_scaled);
  when_key_pressed           = std::move(other.when_key_pressed);
  when_unicode_char_typed    = std::move(other.when_unicode_char_typed);
  when_mouse_button_pressed  = std::move(other.when_mouse_button_pressed);
  when_cursor_moved          = std::move(other.when_cursor_moved);
  when_cursor_entered        = std::move(other.when_cursor_entered);
  when_cursor_exited         = std::move(other.when_cursor_exited);
  when_mouse_scrolled        = std::move(other.when_mouse_scrolled);
  when_file_dropped          = std::move(other.when_file_dropped);
  ptr                        = std::move(other.ptr);

  glfwSetWindowUserPointer(ptr.get(), this);

  return *this;
}

std::strong_ordering glfw_3::window::operator<=>(window const & other) const noexcept
{
  return ptr <=> other.ptr;
}

bool glfw_3::window::should_close() const
{
  return glfwWindowShouldClose(ptr.get());
}

std::string_view glfw_3::window::get_title() const
{
  return glfwGetWindowTitle(ptr.get());
}

void glfw_3::window::set_title(std::string const & title)
{
  glfwSetWindowTitle(ptr.get(), title.c_str());
}

void glfw_3::window::set_icon(std::span<image> images)
{
  std::vector<GLFWimage> glfw_images;

  glfw_images.reserve(images.size());

  for(auto & image : images)
    glfw_images.push_back({
      .height = image.size.height,
      .width  = image.size.width,
      .pixels = reinterpret_cast<unsigned char *>(image.pixels.data()),
    });

  glfwSetWindowIcon(ptr.get(), static_cast<int>(glfw_images.size()), glfw_images.data());
}

glfw_3::int2 glfw_3::window::get_pos() const
{
  int2 pos = {};

  glfwGetWindowPos(ptr.get(), &pos.x, &pos.y);

  return pos;
}

glfw_3::int2 glfw_3::window::get_size() const
{
  int2 size = {};

  glfwGetWindowSize(ptr.get(), &size.width, &size.height);

  return size;
}

glfw_3::int2 glfw_3::window::get_framebuffer_size() const
{
  int2 size = {};

  glfwGetFramebufferSize(ptr.get(), &size.width, &size.height);

  return size;
}

glfw_3::int4 glfw_3::window::get_frame_size() const
{
  int4 size = {};

  glfwGetWindowFrameSize(ptr.get(), &size.left, &size.top, &size.right, &size.bottom);

  return size;
}

glfw_3::float2 glfw_3::window::get_content_scale() const
{
  float2 scale = {};

  glfwGetWindowContentScale(ptr.get(), &scale.x, &scale.y);

  return scale;
}

float glfw_3::window::get_opacity() const
{
  return glfwGetWindowOpacity(ptr.get());
}

glfw_3::monitor const & glfw_3::window::get_monitor() const
{
  return internal::try_emplace_monitor(glfwGetPrimaryMonitor());
}

bool glfw_3::window::get_input_mode(input_mode mode) const
{
  return glfwGetInputMode(ptr.get(), std::to_underlying(mode)) == GLFW_TRUE;
}

glfw_3::action glfw_3::window::get_key(key key) const
{
  return action{glfwGetKey(ptr.get(), std::to_underlying(key))};
}

glfw_3::action glfw_3::window::get_mouse_button(mouse_button button) const
{
  return action{glfwGetMouseButton(ptr.get(), std::to_underlying(button))};
}

glfw_3::double2 glfw_3::window::get_cursor_pos() const
{
  double2 pos = {};

  glfwGetCursorPos(ptr.get(), &pos.x, &pos.y);

  return pos;
}

vk::raii::SurfaceKHR glfw_3::window::create_surface(vk::raii::Instance const &                  instance,
                                                    vk::Optional<vk::AllocationCallbacks const> allocator) const
{
  VkSurfaceKHR surface = VK_NULL_HANDLE;

  auto const result = glfwCreateWindowSurface(*instance, ptr.get(), *allocator, &surface);

  if(result < VK_SUCCESS)
    throw vk::SystemError{vk::make_error_code(vk::Result{result}), "failed to create window surface"};

  return vk::raii::SurfaceKHR(instance, surface, allocator);
}

void glfw_3::window::set_pos(int2 const pos)
{
  glfwSetWindowPos(ptr.get(), pos.x, pos.y);
}

void glfw_3::window::set_size_limits(int2 min, int2 max)
{
  glfwSetWindowSizeLimits(ptr.get(), min.width, min.height, max.width, max.height);
}

void glfw_3::window::set_aspect_ratio(int numer, int denom)
{
  glfwSetWindowAspectRatio(ptr.get(), numer, denom);
}

void glfw_3::window::set_size(int2 size)
{
  glfwSetWindowSize(ptr.get(), size.width, size.height);
}

void glfw_3::window::set_monitor(monitor const & monitor, workarea area, int const refreshRate)
{
  glfwSetWindowMonitor(ptr.get(), monitor.ptr, area.pos.x, area.pos.y, area.size.width, area.size.height, refreshRate);
}

void glfw_3::window::set_input_mode(input_mode mode, bool value)
{
  glfwSetInputMode(ptr.get(), std::to_underlying(mode), value ? GLFW_TRUE : GLFW_FALSE);
}

void glfw_3::window::set_cursor_pos(double2 pos)
{
  glfwSetCursorPos(ptr.get(), pos.x, pos.y);
}

void glfw_3::window::set_cursor(cursor const & cursor)
{
  glfwSetCursor(ptr.get(), cursor.ptr.get());
}

void glfw_3::window::set_clipboard_string(std::string const & str)
{
  glfwSetClipboardString(ptr.get(), str.c_str());
}

void glfw_3::window::set_should_close(bool value)
{
  glfwSetWindowShouldClose(ptr.get(), static_cast<int>(value));
}

void glfw_3::window::set_opacity(float opacity)
{
  glfwSetWindowOpacity(ptr.get(), opacity);
}

void glfw_3::window::minimize()
{
  glfwIconifyWindow(ptr.get());
}

void glfw_3::window::restore()
{
  glfwRestoreWindow(ptr.get());
}

void glfw_3::window::maximize()
{
  glfwMaximizeWindow(ptr.get());
}

void glfw_3::window::show()
{
  glfwShowWindow(ptr.get());
}

void glfw_3::window::hide()
{
  glfwHideWindow(ptr.get());
}

void glfw_3::window::focus()
{
  glfwFocusWindow(ptr.get());
}

void glfw_3::window::request_attention()
{
  glfwRequestWindowAttention(ptr.get());
}

glfw_3::window::window(std::unique_ptr<GLFWwindow, void (*)(GLFWwindow *)> && ptr) noexcept
: ptr(std::move(ptr))
{
  if(ptr)
  {
    glfwSetWindowUserPointer(ptr.get(), this);

    glfwSetWindowPosCallback(ptr.get(), window_pos_callback);

    glfwSetWindowSizeCallback(ptr.get(), window_size_callback);

    glfwSetWindowCloseCallback(ptr.get(), window_close_callback);

    glfwSetWindowRefreshCallback(ptr.get(), window_refresh_callback);

    glfwSetWindowFocusCallback(ptr.get(), window_focus_callback);

    glfwSetWindowIconifyCallback(ptr.get(), window_iconify_callback);

    glfwSetWindowMaximizeCallback(ptr.get(), window_maximize_callback);

    glfwSetFramebufferSizeCallback(ptr.get(), framebuffer_size_callback);

    glfwSetWindowContentScaleCallback(ptr.get(), window_content_scale_callback);

    glfwSetKeyCallback(ptr.get(), key_callback);

    glfwSetCharCallback(ptr.get(), char_callback);

    glfwSetMouseButtonCallback(ptr.get(), mouse_button_callback);

    glfwSetCursorPosCallback(ptr.get(), cursor_pos_callback);

    glfwSetCursorEnterCallback(ptr.get(), cursor_enter_callback);

    glfwSetScrollCallback(ptr.get(), scroll_callback);

    glfwSetDropCallback(ptr.get(), drop_callback);
  }
}
