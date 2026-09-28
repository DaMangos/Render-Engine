#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace graphics
{
std::optional<std::uint32_t> find_memory_type_index(vk::MemoryRequirements const &             memory_requirements,
                                                    vk::PhysicalDeviceMemoryProperties const & memory_properties,
                                                    vk::MemoryPropertyFlags const              memory_property) noexcept;
}