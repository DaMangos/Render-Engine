
#include "library_impl.hpp"
#include "present_window_impl.hpp"

#include <glfw/window.hpp>
#include <graphics/graphical_device.hpp>
#include <graphics/library.hpp>
#include <graphics/render_window.hpp>
#include <logging/logging.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace
{
[[nodiscard]]
static graphics::present_window_impl create_present_window_impl(glfw::window const &               window,
                                                                graphics::vulkan::instance const & instance)
{
  return {
    {instance.as_dependencies(), window.create_surface(instance.get())}
  };
}
}

graphics::present_window::present_window(glfw::window && window, class library const & library)
: glfw::window(std::move(window)),
  self(std::make_unique<present_window_impl>(create_present_window_impl(*this, library.self->instance)))
{
}

graphics::present_window::~present_window() noexcept = default;
