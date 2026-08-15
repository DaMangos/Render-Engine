#include "internal.hpp"

#include <glfw/cursor.hpp>
#include <GLFW/glfw3.h>
#include <glfw/library.hpp>
#include <glfw/monitor.hpp>
#include <glfw/window.hpp>
#include <logging/logging.hpp>

#include <filesystem>
#include <utility>

namespace
{
namespace detail
{
static void window_pos_callback(GLFWwindow * window, int xpos, int ypos)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_moved)
    self->when_window_moved(*self, {.x = xpos, .y = ypos});
}

static void window_size_callback(GLFWwindow * window, int width, int height)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_resized)
    self->when_window_resized(*self, {.width = width, .height = height});
}

static void window_close_callback(GLFWwindow * window)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_closed)
    self->when_window_closed(*self);
}

static void window_refresh_callback(GLFWwindow * window)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_refreshed)
    self->when_window_refreshed(*self);
}

static void window_focus_callback(GLFWwindow * window, int focused)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_focused and focused == VK_TRUE)
    self->when_window_focused(*self);

  if(self and self->when_window_unfocused and focused == VK_FALSE)
    self->when_window_unfocused(*self);
}

static void window_iconify_callback(GLFWwindow * window, int iconified)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_minimized and iconified == VK_TRUE)
    self->when_window_minimized(*self);

  if(self and self->when_window_unminimized and iconified == VK_FALSE)
    self->when_window_unminimized(*self);
}

static void window_maximize_callback(GLFWwindow * window, int maximized)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_maximized and maximized == VK_TRUE)
    self->when_window_maximized(*self);

  if(self and self->when_window_unmaximized and maximized == VK_FALSE)
    self->when_window_unmaximized(*self);
}

static void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_framebuffer_resized)
    self->when_framebuffer_resized(*self, {.width = width, .height = height});
}

static void window_content_scale_callback(GLFWwindow * window, float xscale, float yscale)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_window_content_scaled)
    self->when_window_content_scaled(*self, {.x = xscale, .y = yscale});
}

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_key_pressed)
    self->when_key_pressed(*self, glfw::key{key}, scancode, glfw::action{action}, glfw::modifier{mods});
}

static void char_callback(GLFWwindow * window, unsigned int codepoint)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_unicode_char_typed)
    self->when_unicode_char_typed(*self, static_cast<char32_t>(codepoint));
}

static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_mouse_button_pressed)
    self->when_mouse_button_pressed(*self,
                                    glfw::mouse_button{button},
                                    glfw::action{action},
                                    glfw::modifier{mods});
}

static void cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_cursor_moved)
    self->when_cursor_moved(*self, {.x = xpos, .y = ypos});
}

static void cursor_enter_callback(GLFWwindow * window, int entered)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_cursor_entered and entered == VK_TRUE)
    self->when_cursor_entered(*self);

  if(self and self->when_cursor_exited and entered == VK_FALSE)
    self->when_cursor_exited(*self);
}

static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
  auto * const self = static_cast<glfw::window *>(glfwGetWindowUserPointer(window));

  if(self and self->when_mouse_scrolled)
    self->when_mouse_scrolled(*self, {.x = xoffset, .y = yoffset});
}

static void drop_callback(GLFWwindow * window, int path_count, char const ** paths)
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
}
}

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

bool glfw::window::get_input_mode(input_mode const mode) const
{
  return glfwGetInputMode(ptr.get(), std::to_underlying(mode)) == GLFW_TRUE;
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

vk::raii::SurfaceKHR glfw::window::create_surface(
  vk::raii::Instance const &                        instance,
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
  glfwSetWindowMonitor(ptr.get(),
                       monitor.ptr,
                       area.pos.x,
                       area.pos.y,
                       area.size.width,
                       area.size.height,
                       refreshRate);
}

void glfw::window::set_input_mode(input_mode const mode, bool const value)
{
  glfwSetInputMode(ptr.get(), std::to_underlying(mode), value ? GLFW_TRUE : GLFW_FALSE);
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

glfw::window::window(std::unique_ptr<GLFWwindow, void (*)(GLFWwindow *)> && new_ptr) noexcept
: ptr(std::move(new_ptr))
{
  assert(ptr);

  glfwSetWindowUserPointer(ptr.get(), this);

  glfwSetWindowPosCallback(ptr.get(), ::detail::window_pos_callback);

  glfwSetWindowSizeCallback(ptr.get(), ::detail::window_size_callback);

  glfwSetWindowCloseCallback(ptr.get(), ::detail::window_close_callback);

  glfwSetWindowRefreshCallback(ptr.get(), ::detail::window_refresh_callback);

  glfwSetWindowFocusCallback(ptr.get(), ::detail::window_focus_callback);

  glfwSetWindowIconifyCallback(ptr.get(), ::detail::window_iconify_callback);

  glfwSetWindowMaximizeCallback(ptr.get(), ::detail::window_maximize_callback);

  glfwSetFramebufferSizeCallback(ptr.get(), ::detail::framebuffer_size_callback);

  glfwSetWindowContentScaleCallback(ptr.get(), ::detail::window_content_scale_callback);

  glfwSetKeyCallback(ptr.get(), ::detail::key_callback);

  glfwSetCharCallback(ptr.get(), ::detail::char_callback);

  glfwSetMouseButtonCallback(ptr.get(), ::detail::mouse_button_callback);

  glfwSetCursorPosCallback(ptr.get(), ::detail::cursor_pos_callback);

  glfwSetCursorEnterCallback(ptr.get(), ::detail::cursor_enter_callback);

  glfwSetScrollCallback(ptr.get(), ::detail::scroll_callback);

  glfwSetDropCallback(ptr.get(), ::detail::drop_callback);
}
