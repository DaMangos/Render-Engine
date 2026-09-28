#include "find_memory_type_index.hpp"

std::optional<std::uint32_t> graphics::find_memory_type_index(vk::MemoryRequirements const &             memory_requirements,
                                                              vk::PhysicalDeviceMemoryProperties const & memory_properties,
                                                              vk::MemoryPropertyFlags const memory_property) noexcept
{
  for(std::uint32_t memory_type_index = 0; memory_type_index < memory_properties.memoryTypeCount; memory_type_index++)
    if(memory_requirements.memoryTypeBits & (1u << memory_type_index)
       and (memory_properties.memoryTypes[memory_type_index].propertyFlags & memory_property) == memory_property)
      return memory_type_index;

  return std::nullopt;
}
