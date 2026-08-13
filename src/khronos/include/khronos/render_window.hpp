#pragma once

#include <khronos/present_window.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <memory>
#include <vector>

namespace khronos
{
class render_window : public present_window
{
  private:
    friend class graphical_device;

    render_window(present_window &&                                         window,
                  std::shared_ptr<vk::raii::PhysicalDevice const> const &   physical_device,
                  std::shared_ptr<vk::raii::Device const> const &           device,
                  std::shared_ptr<vk::SwapchainCreateInfoKHR const> const & default_swapchain_create_info,
                  std::shared_ptr<vk::Extent2D const> const &               min_image_extent,
                  std::shared_ptr<vk::Extent2D const> const &               max_image_extent);

    std::shared_ptr<vk::SwapchainCreateInfoKHR>             swapchain_create_info;
    std::shared_ptr<vk::raii::SwapchainKHR const>           swapchain;
    std::vector<std::shared_ptr<vk::raii::ImageView const>> image_views;
    std::shared_ptr<vk::raii::ShaderModule const>           shader_module;
    std::shared_ptr<vk::raii::PipelineLayout const>         pipeline_layout;
    std::shared_ptr<vk::raii::Pipeline const>               graphics_pipeline;
};
}