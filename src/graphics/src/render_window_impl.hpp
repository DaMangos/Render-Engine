#pragma once

#include "vulakn_handles.hpp"

namespace graphics
{
struct render_window_impl
{
    vk::SurfaceCapabilitiesKHR    surface_capabilities;
    vulkan::swapchain_create_info swapchain_create_info;
    vulkan::swapchain             swapchain;
    std::vector<vk::Image>        images;
    vulkan::image_views           image_views;
};
}
