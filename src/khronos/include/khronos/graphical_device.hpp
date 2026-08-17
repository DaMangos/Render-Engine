#pragma once

#include <khronos/fwd.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace khronos
{
class graphical_device
{
  public:
    [[nodiscard]]
    graphics_pipeline create_graphics_pipeline() const;

    [[nodiscard]]
    render_window create_render_window(present_window && window) const;

    [[nodiscard]]
    staging_buffer allocate_staging_buffer(vk::DeviceSize const size) const;

    [[nodiscard]]
    index_transfer_buffer allocate_index_transfer_buffer(vk::DeviceSize const size) const;

    [[nodiscard]]
    vertex_transfer_buffer allocate_vertex_transfer_buffer(vk::DeviceSize const size) const;

    void flush(staging_buffer & staging_buffer);

    void draw(graphics_pipeline &      graphics_pipeline,
              index_transfer_buffer &  index_transfer_buffer,
              vertex_transfer_buffer & vertex_transfer_buffer,
              render_window &          render_window) const;

    graphical_device(graphical_device &&) noexcept = default;

    graphical_device(graphical_device const &) noexcept = delete;

    graphical_device & operator=(graphical_device &&) noexcept = default;

    graphical_device & operator=(graphical_device const &) noexcept = delete;

    ~graphical_device() = default;

  private:
    friend class library;

    graphical_device(std::shared_ptr<vk::raii::Context const> const &    context,
                     std::shared_ptr<vk::raii::Instance const> const &   instance,
                     std::shared_ptr<vk::raii::SurfaceKHR const> const & surface);

    std::shared_ptr<vk::raii::PhysicalDevice const>   physical_device;
    std::shared_ptr<vk::raii::Device const>           device;
    std::shared_ptr<vk::raii::Queue const>            transfer_and_graphics_and_present_queue;
    std::uint32_t                                     transfer_and_graphics_and_present_queue_family_index;
    std::shared_ptr<vk::SwapchainCreateInfoKHR const> default_swapchain_create_info;
};
}