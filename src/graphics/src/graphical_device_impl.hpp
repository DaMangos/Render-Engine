#pragma once

#include "vulakn_handles.hpp"

namespace graphics
{
struct graphical_device_impl
{
    vulkan::physical_device physical_device;
    vulkan::device          device;
    vulkan::queue           queue;
    std::uint32_t           queue_family_index;
};
}