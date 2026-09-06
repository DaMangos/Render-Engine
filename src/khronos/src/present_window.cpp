
#include "impl.hpp"

#include <glfw/window.hpp>
#include <khronos/graphical_device.hpp>
#include <khronos/library.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace
{

[[nodiscard]]
static khronos::present_window_impl create_present_window_impl(glfw::window const & window, khronos::instance const & instance)
{
  return {
    {instance.as_dependencies(), window.create_surface(instance.get())}
  };
}
}

khronos::present_window::present_window(glfw::window && window, class library const & library)
: glfw::window(std::move(window)),
  self(std::make_unique<present_window_impl>(create_present_window_impl(*this, library.self->instance)))
{
}

khronos::present_window::~present_window() noexcept = default;
