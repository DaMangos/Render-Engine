#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace khronos
{
class present_window;
class render_window;

class graphical_device
{
  public:
    render_window create_render_window(present_window && window) const;

    void draw_next_frame(render_window & render_window) const;

    graphical_device(graphical_device &&) noexcept = default;

    graphical_device(graphical_device const &) noexcept = delete;

    graphical_device & operator=(graphical_device &&) noexcept = default;

    graphical_device & operator=(graphical_device const &) noexcept = delete;

    ~graphical_device() = default;

  private:
    friend class library;

    graphical_device(std::shared_ptr<vk::raii::Instance const>           instance,
                     std::shared_ptr<vk::raii::SurfaceKHR const> const & surface);

    std::shared_ptr<vk::raii::PhysicalDevice const>   physical_device;
    std::shared_ptr<vk::raii::Device const>           device;
    std::shared_ptr<vk::raii::Queue const>            graphics_and_present_queue;
    std::uint32_t                                     graphics_and_present_queue_family_index;
    std::shared_ptr<vk::SwapchainCreateInfoKHR const> default_swapchain_create_info;
};
}