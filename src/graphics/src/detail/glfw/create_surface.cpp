#include "create_surface.hpp"

#include "../make_shared_with_deleter_data.hpp"

std::shared_ptr<vk::raii::SurfaceKHR const>  //
graphics::detail::glfw::create_surface(std::shared_ptr<vk::raii::Instance const> const & instance,
                                       std::shared_ptr<GLFWwindow> const &               window)
{
  VkSurfaceKHR surface = VK_NULL_HANDLE;

  auto const result = glfwCreateWindowSurface(**instance, window.get(), nullptr, &surface);

  if(result < VK_SUCCESS)
    throw vk::SystemError{vk::make_error_code(vk::Result{result}), "failed to create window surface"};

  return make_shared_with_deleter_data<vk::raii::SurfaceKHR const>(std::make_tuple(instance, window), *instance, surface);
}
