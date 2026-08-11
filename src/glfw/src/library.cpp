#include "internal.hpp"

#include <glfw_3/cursor.hpp>
#include <glfw_3/library.hpp>
#include <glfw_3/monitor.hpp>
#include <glfw_3/window.hpp>

#include <GLFW/glfw3.h>

#include <set>
#include <stdexcept>
#include <system_error>

glfw_3::library glfw_3::default_library = glfw_3::internal::init_library();

namespace
{
class glfw_category_type : public std::error_category
{
  public:
    [[nodiscard]]
    char const * name() const noexcept override
    {
      return "GLFW";
    }

    [[nodiscard]]
    std::string message(int const error_code) const override
    {
      switch(error_code)
      {
        case GLFW_NOT_INITIALIZED :
          return "GLFW has not been initialized";
        case GLFW_NO_CURRENT_CONTEXT :
          return "No context is current for this thread";
        case GLFW_INVALID_ENUM :
          return "One of the arguments to the function was an invalid enum value";
        case GLFW_INVALID_VALUE :
          return "One of the arguments to the function was an invalid value";
        case GLFW_OUT_OF_MEMORY :
          return "A memory allocation failed";
        case GLFW_API_UNAVAILABLE :
          return "GLFW could not find support for the requested API on the system";
        case GLFW_VERSION_UNAVAILABLE :
          return "The requested OpenGL or OpenGL ES version is not available";
        case GLFW_PLATFORM_ERROR :
          return "A platform-specific error occurred that does not match any of the more specific categories";
        case GLFW_FORMAT_UNAVAILABLE :
          return "The specified format is not supported or available";
        case GLFW_NO_WINDOW_CONTEXT :
          return "The specified _window does not have an OpenGL or OpenGL ES context";
        case GLFW_CURSOR_UNAVAILABLE :
          return "The specified cursor shape is not available";
        case GLFW_FEATURE_UNAVAILABLE :
          return "The requested feature is not provided by the platform";
        case GLFW_FEATURE_UNIMPLEMENTED :
          return "The requested feature is not implemented for the platform";
        case GLFW_PLATFORM_UNAVAILABLE :
          return "Platform unavailable or no matching platform was found";
        default :
          throw std::invalid_argument("unknow GLFW error code: " + std::to_string(error_code));
      }
    }
};

[[noreturn]]
static void error_callback(int error_code, char const * description)
{
  static glfw_category_type const category;
  throw std::system_error(error_code, category, description);
}

static void monitor_callback(GLFWmonitor * glfw_monitor, int connected)
{
  auto const & found = glfw_3::internal::try_emplace_monitor(glfw_monitor);

  if(found.when_monitor_connected and connected == VK_TRUE)
    glfw_3::default_library.when_monitor_connected(found);

  if(found.when_monitor_disconnected and connected == VK_FALSE)
    glfw_3::default_library.when_monitor_disconnected(found);
}
}

glfw_3::library::library()
{
  static bool has_default_library_been_initialized = false;

  if(has_default_library_been_initialized)
    throw std::logic_error("glfw_3 has already been init");

  has_default_library_been_initialized = true;

  glfwInitHint(GLFW_CLIENT_API, GLFW_NO_API);

  glfwSetErrorCallback(error_callback);

  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  glfwSetMonitorCallback(monitor_callback);
}

glfw_3::library::~library()
{
  glfwTerminate();
}

glfw_3::window glfw_3::library::create_window(int2 size, std::string const & title) const
{
  return window({glfwCreateWindow(size.width, size.height, title.c_str(), nullptr, nullptr), glfwDestroyWindow});
}

glfw_3::window glfw_3::library::create_window(int2 size, std::string const & title, window const & share) const
{
  return window(
    {glfwCreateWindow(size.width, size.height, title.c_str(), nullptr, share.ptr.get()), glfwDestroyWindow});
}

glfw_3::window glfw_3::library::create_window(int2 size, std::string const & title, monitor const & monitor) const
{
  return window({glfwCreateWindow(size.width, size.height, title.c_str(), monitor.ptr, nullptr), glfwDestroyWindow});
}

glfw_3::window glfw_3::library::create_window(int2                size,
                                              std::string const & title,
                                              window const &      share,
                                              monitor const &     monitor) const
{
  return window(
    {glfwCreateWindow(size.width, size.height, title.c_str(), monitor.ptr, share.ptr.get()), glfwDestroyWindow});
}

std::set<glfw_3::monitor> const & glfw_3::library::get_monitors() const
{
  int            count         = 0;
  GLFWmonitor ** glfw_monitors = glfwGetMonitors(&count);

  if(not glfw_monitors)
    return internal::monitors;

  for(auto const & glfw_monitor : std::span{glfw_monitors, std::ranges::next(glfw_monitors, count)})
    internal::try_emplace_monitor(glfw_monitor);

  return internal::monitors;
}

glfw_3::monitor const & glfw_3::library::get_primary_monitor() const
{
  return internal::try_emplace_monitor(glfwGetPrimaryMonitor());
}

glfw_3::cursor glfw_3::library::create_cursor(image image, int2 hotspot) const
{
  GLFWimage glfw_image = {
    .height = image.size.height,
    .width  = image.size.width,
    .pixels = reinterpret_cast<unsigned char *>(image.pixels.data()),
  };

  return cursor({glfwCreateCursor(&glfw_image, hotspot.x, hotspot.y), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_arrow_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_ARROW_CURSOR), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_ibeam_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_IBEAM_CURSOR), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_crosshair_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_pointing_hand_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_resize_ew_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_resize_ns_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_resize_nwse_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_resize_nesw_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_resize_all_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR), glfwDestroyCursor});
}

glfw_3::cursor glfw_3::library::create_not_allowed_cursor() const
{
  return cursor({glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR), glfwDestroyCursor});
}

std::span<char const * const> glfw_3::library::get_required_instance_extensions() const
{
  std::uint32_t count      = 0;
  char const ** extensions = glfwGetRequiredInstanceExtensions(&count);

  assert(extensions);

  return {extensions, static_cast<std::size_t>(count)};
}

void glfw_3::library::poll_events() const
{
  glfwPollEvents();
}