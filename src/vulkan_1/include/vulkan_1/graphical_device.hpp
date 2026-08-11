#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace vulkan_1
{
class library;

class graphical_device
{
  public:
    class render_window create_render_window(class present_window && window) const;

    graphical_device(graphical_device &&) noexcept = default;

    graphical_device(graphical_device const &) noexcept = delete;

    graphical_device & operator=(graphical_device &&) noexcept = default;

    graphical_device & operator=(graphical_device const &) noexcept = delete;

    ~graphical_device() = default;

  private:
    friend library;

    graphical_device(std::shared_ptr<vk::raii::Instance const>           instance,
                     std::shared_ptr<vk::raii::SurfaceKHR const> const & surface);

    std::shared_ptr<vk::raii::PhysicalDevice const>   physical_device;
    std::shared_ptr<vk::raii::Device const>           device;
    std::shared_ptr<vk::raii::Queue const>            graphics_queue;
    std::shared_ptr<vk::raii::Queue const>            present_queue;
    std::shared_ptr<vk::SwapchainCreateInfoKHR const> default_swapchain_create_info;
    std::shared_ptr<vk::Extent2D const>               min_image_extent;
    std::shared_ptr<vk::Extent2D const>               max_image_extent;
};
}