#include "create_window.hpp"

#include "init_library.hpp"

std::shared_ptr<GLFWwindow> graphics::detail::glfw::create_window(int           width,
                                                                  int           height,
                                                                  char const *  title,
                                                                  GLFWmonitor * monitor,
                                                                  GLFWwindow *  share)
{
  auto const library = init_library();

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  return {glfwCreateWindow(width, height, title, monitor, share),
          [library](GLFWwindow * window) { glfwDestroyWindow(window); }};
}
