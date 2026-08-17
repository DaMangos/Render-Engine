#pragma once

#include <khronos/fwd.hpp>
#include <khronos/present_window.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <memory>

namespace khronos
{
class graphics_pipeline
{
  public:

  private:
    friend class graphical_device;

    graphics_pipeline(std::shared_ptr<vk::raii::Device const> const &           device,
                      std::shared_ptr<vk::SwapchainCreateInfoKHR const> const & swapchain_create_info);

    std::shared_ptr<vk::raii::PipelineLayout const> pipeline_layout;
    std::shared_ptr<vk::raii::Pipeline const>       pipeline;
};
}
