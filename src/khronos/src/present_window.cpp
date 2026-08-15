#include "detail/make_shared_with_data.hpp"

#include <khronos/present_window.hpp>

khronos::present_window::present_window(glfw::window &&                                   window,
                                        std::shared_ptr<vk::raii::Instance const> const & instance)
: glfw::window(std::move(window)),
  surface(detail::make_shared_with_data<vk::raii::SurfaceKHR const>(glfw::window::create_surface(*instance)))
{
  detail::emplace_data(surface, instance);
}