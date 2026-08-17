#include "internal.hpp"

#include <glfw/cursor.hpp>
#include <glfw/library.hpp>
#include <glfw/monitor.hpp>
#include <glfw/window.hpp>
#include <logging/logging.hpp>

#include <filesystem>
#include <utility>

glfw::window::window(window && other) noexcept
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

glfw::window & glfw::window::operator=(window && other) noexcept
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

std::strong_ordering glfw::window::operator<=>(window const & other) const noexcept
{
  return ptr <=> other.ptr;
}

std::string_view glfw::window::get_title() const
{
  return glfwGetWindowTitle(ptr.get());
}

void glfw::window::set_title(std::string const & title)
{
  glfwSetWindowTitle(ptr.get(), title.c_str());
}

void glfw::window::set_icon(std::span<image> const images)
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

glfw::coordinates<int, 2> glfw::window::get_pos() const
{
  coordinates<int, 2> pos = {};

  glfwGetWindowPos(ptr.get(), &pos.x, &pos.y);

  return pos;
}

glfw::dimensions<int, 2> glfw::window::get_size() const
{
  dimensions<int, 2> size = {};

  glfwGetWindowSize(ptr.get(), &size.width, &size.height);

  return size;
}

glfw::dimensions<int, 2> glfw::window::get_framebuffer_size() const
{
  dimensions<int, 2> size = {};

  glfwGetFramebufferSize(ptr.get(), &size.width, &size.height);

  return size;
}

glfw::edges<int> glfw::window::get_frame_size() const
{
  edges<int> edges = {};

  glfwGetWindowFrameSize(ptr.get(), &edges.left, &edges.top, &edges.right, &edges.bottom);

  return edges;
}

glfw::coordinates<float, 2> glfw::window::get_content_scale() const
{
  coordinates<float, 2> scale = {};

  glfwGetWindowContentScale(ptr.get(), &scale.x, &scale.y);

  return scale;
}

float glfw::window::get_opacity() const
{
  return glfwGetWindowOpacity(ptr.get());
}

glfw::monitor const & glfw::window::get_monitor() const
{
  return *internal::try_emplace_monitor(glfwGetPrimaryMonitor());
}

bool glfw::window::is_input_mode_cursor() const
{
  return glfwGetInputMode(ptr.get(), GLFW_CURSOR) == GLFW_TRUE;
}

bool glfw::window::is_input_mode_sticky_keys() const
{
  return glfwGetInputMode(ptr.get(), GLFW_STICKY_KEYS) == GLFW_TRUE;
}

bool glfw::window::is_input_mode_sticky_mouse_buttons() const
{
  return glfwGetInputMode(ptr.get(), GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

bool glfw::window::is_input_mode_lock_key_mods() const
{
  return glfwGetInputMode(ptr.get(), GLFW_LOCK_KEY_MODS) == GLFW_TRUE;
}

bool glfw::window::is_input_mode_raw_mouse_motion() const
{
  return glfwGetInputMode(ptr.get(), GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE;
}

glfw::action glfw::window::get_key(key const key) const
{
  return action{glfwGetKey(ptr.get(), std::to_underlying(key))};
}

glfw::action glfw::window::get_mouse_button(mouse_button const button) const
{
  return action{glfwGetMouseButton(ptr.get(), std::to_underlying(button))};
}

glfw::coordinates<double, 2> glfw::window::get_cursor_pos() const
{
  coordinates<double, 2> pos = {};

  glfwGetCursorPos(ptr.get(), &pos.x, &pos.y);

  return pos;
}

vk::raii::SurfaceKHR glfw::window::create_surface(vk::raii::Instance const &                        instance,
                                                  vk::Optional<vk::AllocationCallbacks const> const allocator) const
{
  VkSurfaceKHR surface = VK_NULL_HANDLE;

  auto const result = vk::Result{glfwCreateWindowSurface(*instance, ptr.get(), *allocator, &surface)};

  if(result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(result), "failed to create window surface"};

  if(result > vk::Result::eSuccess)
    logging::warning() << "create window surface returned a warning: " << vk::to_string(vk::Result{result});

  return vk::raii::SurfaceKHR(instance, surface, allocator);
}

bool glfw::window::should_close() const
{
  return glfwWindowShouldClose(ptr.get()) == VK_TRUE;
}

bool glfw::window::is_focused() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_FOCUSED) == GLFW_TRUE;
}

bool glfw::window::is_minimized() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_ICONIFIED) == GLFW_TRUE;
}

bool glfw::window::is_maximized() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_MAXIMIZED) == GLFW_TRUE;
}

bool glfw::window::is_cursor_hovered() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_HOVERED) == GLFW_TRUE;
}

bool glfw::window::is_visible() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_VISIBLE) == GLFW_TRUE;
}

bool glfw::window::is_resizable() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_RESIZABLE) == GLFW_TRUE;
}

bool glfw::window::is_decorated() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_DECORATED) == GLFW_TRUE;
}

bool glfw::window::is_auto_minimized() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_AUTO_ICONIFY) == GLFW_TRUE;
}

bool glfw::window::is_floating() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_FLOATING) == GLFW_TRUE;
}

bool glfw::window::is_transparent_framebuffer() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_TRANSPARENT_FRAMEBUFFER) == GLFW_TRUE;
}

bool glfw::window::is_focus_on_show() const
{
  return glfwGetWindowAttrib(ptr.get(), GLFW_FOCUS_ON_SHOW) == GLFW_TRUE;
}

void glfw::window::set_pos(coordinates<int, 2> const & pos)
{
  glfwSetWindowPos(ptr.get(), pos.x, pos.y);
}

void glfw::window::set_size_limits(dimensions<int, 2> const & min, dimensions<int, 2> const & max)
{
  glfwSetWindowSizeLimits(ptr.get(), min.width, min.height, max.width, max.height);
}

void glfw::window::set_aspect_ratio(int const numer, int const denom)
{
  glfwSetWindowAspectRatio(ptr.get(), numer, denom);
}

void glfw::window::set_size(dimensions<int, 2> const & size)
{
  glfwSetWindowSize(ptr.get(), size.width, size.height);
}

void glfw::window::set_monitor(monitor const & monitor, workarea const & area, int const refreshRate)
{
  glfwSetWindowMonitor(ptr.get(), monitor.ptr, area.pos.x, area.pos.y, area.size.width, area.size.height, refreshRate);
}

void glfw::window::set_input_mode_cursor(bool const value)
{
  glfwSetInputMode(ptr.get(), GLFW_CURSOR, value ? GLFW_TRUE : GLFW_FALSE);
}

void glfw::window::set_input_mode_sticky_keys(bool const value)
{
  glfwSetInputMode(ptr.get(), GLFW_STICKY_KEYS, value ? GLFW_TRUE : GLFW_FALSE);
}

void glfw::window::set_input_mode_sticky_mouse_buttons(bool const value)
{
  glfwSetInputMode(ptr.get(), GLFW_STICKY_MOUSE_BUTTONS, value ? GLFW_TRUE : GLFW_FALSE);
}

void glfw::window::set_input_mode_lock_key_mods(bool const value)
{
  glfwSetInputMode(ptr.get(), GLFW_LOCK_KEY_MODS, value ? GLFW_TRUE : GLFW_FALSE);
}

void glfw::window::set_input_mode_raw_mouse_motion(bool const value)
{
  glfwSetInputMode(ptr.get(), GLFW_RAW_MOUSE_MOTION, value ? GLFW_TRUE : GLFW_FALSE);
}

void glfw::window::set_cursor_pos(coordinates<double, 2> const & pos)
{
  glfwSetCursorPos(ptr.get(), pos.x, pos.y);
}

void glfw::window::set_cursor(cursor const & cursor)
{
  glfwSetCursor(ptr.get(), cursor.ptr.get());
}

void glfw::window::set_clipboard_string(std::string const & str)
{
  glfwSetClipboardString(ptr.get(), str.c_str());
}

void glfw::window::set_should_close(bool const value)
{
  glfwSetWindowShouldClose(ptr.get(), static_cast<int>(value));
}

void glfw::window::set_opacity(float const opacity)
{
  glfwSetWindowOpacity(ptr.get(), opacity);
}

void glfw::window::minimize()
{
  glfwIconifyWindow(ptr.get());
}

void glfw::window::restore()
{
  glfwRestoreWindow(ptr.get());
}

void glfw::window::maximize()
{
  glfwMaximizeWindow(ptr.get());
}

void glfw::window::show()
{
  glfwShowWindow(ptr.get());
}

void glfw::window::hide()
{
  glfwHideWindow(ptr.get());
}

void glfw::window::focus()
{
  glfwFocusWindow(ptr.get());
}

void glfw::window::request_attention()
{
  glfwRequestWindowAttention(ptr.get());
}

void glfw::window::close() noexcept
{
  ptr.reset();
}

glfw::window::window(std::unique_ptr<GLFWwindow, void (*)(GLFWwindow *)> && new_ptr) noexcept
: ptr(std::move(new_ptr))
{
  assert(ptr);

  glfwSetWindowUserPointer(ptr.get(), this);

  glfwSetWindowPosCallback(ptr.get(), internal::window_pos_callback);

  glfwSetWindowSizeCallback(ptr.get(), internal::window_size_callback);

  glfwSetWindowCloseCallback(ptr.get(), internal::window_close_callback);

  glfwSetWindowRefreshCallback(ptr.get(), internal::window_refresh_callback);

  glfwSetWindowFocusCallback(ptr.get(), internal::window_focus_callback);

  glfwSetWindowIconifyCallback(ptr.get(), internal::window_iconify_callback);

  glfwSetWindowMaximizeCallback(ptr.get(), internal::window_maximize_callback);

  glfwSetFramebufferSizeCallback(ptr.get(), internal::framebuffer_size_callback);

  glfwSetWindowContentScaleCallback(ptr.get(), internal::window_content_scale_callback);

  glfwSetKeyCallback(ptr.get(), internal::key_callback);

  glfwSetCharCallback(ptr.get(), internal::char_callback);

  glfwSetMouseButtonCallback(ptr.get(), internal::mouse_button_callback);

  glfwSetCursorPosCallback(ptr.get(), internal::cursor_pos_callback);

  glfwSetCursorEnterCallback(ptr.get(), internal::cursor_enter_callback);

  glfwSetScrollCallback(ptr.get(), internal::scroll_callback);

  glfwSetDropCallback(ptr.get(), internal::drop_callback);
}
