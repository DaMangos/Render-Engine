#include "detail/make_shared_with_data.hpp"

#include <vulkan_1/present_window.hpp>

vulkan_1::present_window::present_window(glfw_3::window &&                                 window,
                                         std::shared_ptr<vk::raii::Instance const> const & instance)
: glfw_3::window(std::move(window)),
  surface(
    detail::make_shared_with_data<vk::raii::SurfaceKHR const>(instance, glfw_3::window::create_surface(*instance)))
{
}