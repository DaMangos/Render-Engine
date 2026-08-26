#pragma once

#include "dependencies.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <list>
#include <ostream>
#include <set>
#include <unordered_map>

namespace khronos
{
struct user_data
{
    std::ostream * vk_verbose_out;
    std::ostream * vk_info_out;
    std::ostream * vk_warning_out;
    std::ostream * vk_error_out;
};

using context               = dependent<vk::raii::Context>;
using instance              = dependent<vk::raii::Instance, dependencies<vk::raii::Context, user_data>>;
using surface               = dependent<vk::raii::SurfaceKHR, instance::as_dependencies_type>;
using debug_utils_messenger = dependent<vk::raii::DebugUtilsMessengerEXT, instance::as_dependencies_type>;
using physical_device       = dependent<vk::raii::PhysicalDevice, instance::as_dependencies_type>;
using device                = dependent<vk::raii::Device, physical_device::as_dependencies_type>;
using queue                 = dependent<vk::raii::Queue, device::as_dependencies_type>;
using pipeline_layout       = dependent<vk::raii::PipelineLayout, device::as_dependencies_type>;
using pipeline              = dependent<vk::raii::Pipeline, pipeline_layout::as_dependencies_type>;
using fences                = dependent<std::list<vk::raii::Fence>, queue::as_dependencies_type>;
using semaphores            = dependent<std::list<vk::raii::Semaphore>, queue::as_dependencies_type>;
using command_pool          = dependent<vk::raii::CommandPool, fences::as_dependencies_type>;
using command_buffers       = dependent<std::list<vk::raii::CommandBuffer>, command_pool::as_dependencies_type>;
using swapchain_create_info = dependent<vk::SwapchainCreateInfoKHR, surface::as_dependencies_type>;

using swapchain = dependent<
  vk::raii::SwapchainKHR,
  dependency_union_type<queue::as_dependencies_type, swapchain_create_info::as_dependencies_type>>;

using image_views   = dependent<std::vector<vk::raii::ImageView>, swapchain::as_dependencies_type>;
using buffer        = dependent<vk::raii::Buffer, device::as_dependencies_type>;
using device_memory = dependent<vk::raii::DeviceMemory, buffer::as_dependencies_type>;

struct library_impl
{
    context                              context;
    instance                             instance;
    std::optional<debug_utils_messenger> debug_utils_messenger;
};

struct present_window_impl
{
    surface surface;
};

struct graphical_device_impl
{
    physical_device physical_device;
    device          device;
    queue           queue;
    std::uint32_t   queue_family_index;
};

struct graphics_pipeline_impl
{
    pipeline_layout pipeline_layout;
    pipeline        pipeline;
};

struct draw_command_impl
{
    fences          fences;
    semaphores      render_complete_semaphores;
    semaphores      present_complete_semaphores;
    command_pool    command_pool;
    command_buffers command_buffers;
};

struct transfer_command_impl
{
    fences          fences;
    command_pool    command_pool;
    command_buffers command_buffers;
};

struct render_window_impl
{
    vk::SurfaceCapabilitiesKHR surface_capabilities;
    swapchain_create_info      swapchain_create_info;
    swapchain                  swapchain;
    std::vector<vk::Image>     images;
    image_views                image_views;
};

struct staging_buffer_impl
{
    buffer                                     buffer;
    device_memory                              device_memory;
    vk::DeviceSize                             capacity;
    std::set<vk::StridedDeviceAddressRangeKHR> occupied_regions;

    std::unordered_map<vk::BufferUsageFlagBits, std::set<vk::StridedDeviceAddressRangeKHR>::const_iterator>
      usage_to_occupied_region;

    std::byte * mapped_memory;
};

struct transfer_buffer_impl
{
    buffer                                     buffer;
    device_memory                              device_memory;
    vk::DeviceSize                             capacity;
    std::set<vk::StridedDeviceAddressRangeKHR> occupied_regions;

    std::unordered_map<vk::BufferUsageFlagBits, std::set<vk::StridedDeviceAddressRangeKHR>::const_iterator>
      usage_to_occupied_region;
};
}
