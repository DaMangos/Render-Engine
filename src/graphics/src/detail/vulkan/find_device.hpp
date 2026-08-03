#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <memory>
#include <set>

namespace graphics::detail::vulkan
{
struct find_device_return_type
{
    std::shared_ptr<vk::raii::PhysicalDevice const> physical_device;
    std::shared_ptr<vk::raii::Device const>         device;
    std::shared_ptr<vk::raii::Queue const>          graphics_queue;
    std::shared_ptr<vk::raii::Queue const>          present_queue;
};

[[nodiscard]]
find_device_return_type find_device(std::shared_ptr<vk::raii::Instance const> const &   instance,
                                    std::shared_ptr<vk::raii::SurfaceKHR const> const & surface,
                                    std::set<std::string>                               device_extensions);
}