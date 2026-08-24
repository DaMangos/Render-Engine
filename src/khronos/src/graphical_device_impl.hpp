#pragma once

#include "library_impl.hpp"

namespace khronos
{
using physical_device = dependent<vk::raii::PhysicalDevice, instance::as_dependencies_type>;
using device          = dependent<vk::raii::Device, physical_device::as_dependencies_type>;
using queue           = dependent<vk::raii::Queue, device::as_dependencies_type>;

struct graphical_device_impl
{
    physical_device physical_device;
    device          device;
    queue           queue;
    std::uint32_t   queue_family_index;
};
}
