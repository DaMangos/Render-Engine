#pragma once

#include <khronos/fwd.hpp>
#include <khronos/present_window.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <list>
#include <memory>
#include <vector>

namespace khronos
{
class render_window : public present_window
{
  public:

  private:
    friend class graphical_device;

    render_window(present_window &&                                         window,
                  std::shared_ptr<vk::raii::PhysicalDevice const> const &   physical_device,
                  std::shared_ptr<vk::raii::Device const> const &           device,
                  std::shared_ptr<vk::SwapchainCreateInfoKHR const> const & default_swapchain_create_info,
                  std::shared_ptr<vk::raii::Queue const> const &            graphics_and_present_queue,
                  std::uint32_t const                                       graphics_and_present_queue_family_index);

    std::shared_ptr<vk::SwapchainCreateInfoKHR>   swapchain_create_info;
    std::shared_ptr<vk::raii::SwapchainKHR const> swapchain;

    struct image
    {
        vk::Image                                  image;
        std::shared_ptr<vk::raii::ImageView const> image_view;
        std::shared_ptr<vk::raii::Semaphore const> render_complete_semaphores;
    };

    std::vector<image> images;

    std::shared_ptr<vk::raii::CommandPool const> command_pool;

    struct frame
    {
        std::shared_ptr<vk::raii::CommandBuffer const> command_buffer;
        std::shared_ptr<vk::raii::Fence const>         in_flight_fence;
        std::shared_ptr<vk::raii::Semaphore const>     present_complete_semaphores;
    };

    std::list<frame> frames;
};
}