#include "window_impl.hpp"

#include "glfw/create_surface.hpp"
#include "glfw/create_window.hpp"
#include "vulkan/find_device.hpp"
#include "vulkan/init_library.hpp"

graphics::detail::window_impl::window_impl(pixel const          width,
                                           pixel const          hight,
                                           std::string const &  title,
                                           std::ostream * const vk_verbose_out,
                                           std::ostream * const vk_info_out,
                                           std::ostream * const vk_warning_out,
                                           std::ostream * const vk_error_out)
{
  self.window = glfw::create_window(static_cast<int>(width), static_cast<int>(hight), title.c_str(), nullptr, nullptr);

  std::uint32_t              glfw_extension_count = 0;
  char const * const * const glfw_extensions      = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  auto const & [context, instance, debug_utils_messenger] = vulkan::init_library(  //
    {glfw_extensions, std::ranges::next(glfw_extensions, glfw_extension_count)},   //
    {},                                                                            //
    vk_verbose_out,                                                                //
    vk_info_out,                                                                   //
    vk_warning_out,                                                                //
    vk_error_out);

  self.context               = context;
  self.instance              = instance;
  self.debug_utils_messenger = debug_utils_messenger;
  self.surface               = glfw::create_surface(self.instance, self.window);

  auto const & [physical_device, device, graphics_queue, present_queue] = vulkan::find_device(self.instance, self.surface, {});

  self.physical_device = physical_device;
  self.device          = device;
  self.graphics_queues = graphics_queue;
  self.present_queues  = present_queue;
}

graphics::detail::window_impl::window_impl(window_impl && other)
: self(std::move(other.self))
{
  if(self.window)
    glfwSetWindowUserPointer(self.window.get(), this);
}

graphics::detail::window_impl & graphics::detail::window_impl::operator=(window_impl && other)
{
  self = std::move(other.self);

  if(self.window)
    glfwSetWindowUserPointer(self.window.get(), this);

  return *this;
}

bool graphics::detail::window_impl::should_close() const
{
  return self.window and glfwWindowShouldClose(self.window.get());
}

void graphics::detail::window_impl::poll_events() const
{
  glfwPollEvents();
}

void graphics::detail::window_impl::close() noexcept
{
  self.window.reset();
}