#include "init_library.hpp"

#include <GLFW/glfw3.h>

#include <system_error>

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

class glfw_library
{
  public:
    glfw_library()
    {
      glfwSetErrorCallback(
        [](int const error_code, char const * const description)
        {
          static glfw_category_type const category;
          throw std::system_error(error_code, category, description);
        });

      glfwInit();
    }

    ~glfw_library()
    {
      glfwTerminate();
    }

    glfw_library(glfw_library &&) = delete;

    glfw_library(glfw_library const &) = delete;

    glfw_library & operator=(glfw_library &&) = delete;

    glfw_library & operator=(glfw_library const &) = delete;
};
}

std::shared_ptr<void> graphics::detail::glfw::init_library()
{
  static std::weak_ptr<glfw_library> default_library;

  if(not default_library.expired())
    return default_library.lock();

  auto library = std::make_shared<glfw_library>();

  default_library = library;

  return library;
}
