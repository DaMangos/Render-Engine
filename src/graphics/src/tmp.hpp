#pragma once

#include "vulakn_handles.hpp"

namespace graphics
{
struct library_impl
{
    vulkan::context                              context;
    vulkan::instance                             instance;
    std::optional<vulkan::debug_utils_messenger> debug_utils_messenger;
};

struct present_window_impl
{
    vulkan::surface surface;
};

struct graphical_device_impl
{
    vulkan::physical_device physical_device;
    vulkan::device          device;
    vulkan::queue           queue;
    std::uint32_t           queue_family_index;
};

struct graphics_pipeline_impl
{
    vulkan::pipeline_layout pipeline_layout;
    vulkan::pipeline        pipeline;
};

struct draw_command_impl
{
    vulkan::fences          fences;
    vulkan::semaphores      render_complete_semaphores;
    vulkan::semaphores      present_complete_semaphores;
    vulkan::command_pool    command_pool;
    vulkan::command_buffers command_buffers;
};

struct transfer_command_impl
{
    vulkan::fences          fences;
    vulkan::command_pool    command_pool;
    vulkan::command_buffers command_buffers;
};

struct render_window_impl
{
    vk::SurfaceCapabilitiesKHR    surface_capabilities;
    vulkan::swapchain_create_info swapchain_create_info;
    vulkan::swapchain             swapchain;
    std::vector<vk::Image>        images;
    vulkan::image_views           image_views;
};

struct meshlet_address_range
{
    vk::StridedDeviceAddressRangeKHR vertex_indices;
};

struct mesh_address_range
{
    std::vector<meshlet_address_range> meshlets;
    vk::StridedDeviceAddressRangeKHR   vertices;
    vk::StridedDeviceAddressRangeKHR   textures;
};

struct host_visible_buffer
{
    vulkan::buffer        buffer;
    vulkan::device_memory device_memory;
    std::span<std::byte>  mapped_memory;
};

struct device_local_buffer
{
    vulkan::buffer        buffer;
    vulkan::device_memory device_memory;
};

struct mesh_buffer_impl
{
    host_visible_buffer staging_buffer;
    device_local_buffer transfer_buffer;

    vulkan::fence          fence;
    vulkan::command_pool   command_pool;
    vulkan::command_buffer command_buffer;
};

}
