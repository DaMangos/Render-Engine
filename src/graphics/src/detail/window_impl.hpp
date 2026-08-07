#pragma once

#include "vulkan/vulkan_raii.hpp"

#include <graphics/pixel.hpp>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>

#include <memory>
#include <string>

namespace graphics::detail
{
class window_impl
{
  public:
    window_impl(pixel const          width,
                pixel const          hight,
                std::string const &  title,
                std::ostream * const vk_verbose_out,
                std::ostream * const vk_info_out,
                std::ostream * const vk_warning_out,
                std::ostream * const vk_error_out);

    window_impl(window_impl && other);

    window_impl(window_impl const & other) = delete;

    window_impl & operator=(window_impl && other);

    window_impl & operator=(window_impl const & other) = delete;

    ~window_impl() = default;

    [[nodiscard]]
    bool should_close() const;

    void poll_events() const;

    void close() noexcept;

    struct
    {
        std::shared_ptr<GLFWwindow>                                                               window;
        std::shared_ptr<vk::raii::Context const>                                                  context;
        std::shared_ptr<vk::raii::Instance const>                                                 instance;
        std::shared_ptr<vk::raii::DebugUtilsMessengerEXT const>                                   debug_utils_messenger;
        std::shared_ptr<vk::raii::SurfaceKHR const>                                               surface;
        std::shared_ptr<vk::raii::PhysicalDevice const>                                           physical_device;
        std::shared_ptr<vk::raii::Device const>                                                   device;
        std::shared_ptr<vk::raii::Queue const>                                                    graphics_queues;
        std::shared_ptr<vk::raii::Queue const>                                                    present_queues;
        std::shared_ptr<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D> const> default_swapchain_data;
        std::shared_ptr<vk::raii::SwapchainKHR const>                                             swapchain;
    } self;
};
}
