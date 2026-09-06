#include "buffer.hpp"

#include <sstream>

namespace
{
[[nodiscard]]
static vk::raii::Buffer create_buffer(vk::raii::Device const &   device,
                                      vk::BufferUsageFlags const usage,
                                      vk::DeviceSize const       capacity)
{
  auto const buffer_create_info = vk::BufferCreateInfo{}  //
                                    .setSize(capacity)
                                    .setUsage(usage)
                                    .setSharingMode(vk::SharingMode::eExclusive);

  return {device, buffer_create_info};
}

[[nodiscard]]
static constexpr std::optional<std::uint32_t> find_memory_type_index(
  vk::MemoryRequirements const &             memory_requirements,
  vk::PhysicalDeviceMemoryProperties const & memory_properties,
  vk::MemoryPropertyFlags const              memory_property) noexcept
{
  for(std::uint32_t memory_type_index = 0; memory_type_index < memory_properties.memoryTypeCount; memory_type_index++)
    if(memory_requirements.memoryTypeBits & (1u << memory_type_index)
       and (memory_properties.memoryTypes[memory_type_index].propertyFlags & memory_property) == memory_property)
      return memory_type_index;

  return std::nullopt;
}

[[nodiscard]]
static vk::raii::DeviceMemory allocate_device_memory(vk::raii::Device const &       device,
                                                     std::uint32_t                  memory_type_index,
                                                     vk::MemoryRequirements const & memory_requirements)
{
  auto const memory_allocate_info = vk::MemoryAllocateInfo{}  //
                                      .setMemoryTypeIndex(memory_type_index)
                                      .setAllocationSize(memory_requirements.size);

  return {device, memory_allocate_info};
}
}

khronos::staging_buffer_impl khronos::create_staging_buffer_impl(physical_device const & physical_device,
                                                                 device const &          device,
                                                                 vk::DeviceSize const    capacity)
{
  auto const usage = vk::BufferUsageFlagBits::eTransferSrc;

  auto buffer = khronos::buffer{device.as_dependencies(), create_buffer(device.get(), usage, capacity)};

  auto const memory_properties = physical_device.get().getMemoryProperties();

  auto const memory_requirements = buffer.get().getMemoryRequirements();

  auto const memory_property = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

  auto const memory_type_index = find_memory_type_index(memory_requirements, memory_properties, memory_property);

  if(not memory_type_index)
    throw std::runtime_error((std::stringstream{} << "cannot find a memory properties " << vk::to_string(memory_property)
                                                  << " for usage " << vk::to_string(usage))
                               .str());

  auto device_memory = khronos::device_memory{buffer.as_dependencies(),
                                              allocate_device_memory(device.get(), *memory_type_index, memory_requirements)};

  buffer.get().bindMemory(device_memory.get(), 0);

  auto const mapped_buffer = static_cast<std::byte *>(device_memory.get().mapMemory(0, capacity));

  return {std::move(buffer), std::move(device_memory), capacity, {}, {}, mapped_buffer};
}

khronos::transfer_buffer_impl khronos::create_transfer_buffer_impl(physical_device const & physical_device,
                                                                   device const &          device,
                                                                   vk::DeviceSize const    capacity)
{
  auto const usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer
                   | vk::BufferUsageFlagBits::eVertexBuffer;

  auto buffer = khronos::buffer{device.as_dependencies(), create_buffer(device.get(), usage, capacity)};

  auto const memory_properties = physical_device.get().getMemoryProperties();

  auto const memory_requirements = buffer.get().getMemoryRequirements();

  auto const memory_property = vk::MemoryPropertyFlagBits::eDeviceLocal;

  auto const memory_type_index = find_memory_type_index(memory_requirements, memory_properties, memory_property);

  if(not memory_type_index)
    throw std::runtime_error((std::stringstream{} << "cannot find a memory properties " << vk::to_string(memory_property)
                                                  << " for usage " << vk::to_string(usage))
                               .str());

  auto device_memory = khronos::device_memory{buffer.as_dependencies(),
                                              allocate_device_memory(device.get(), *memory_type_index, memory_requirements)};

  buffer.get().bindMemory(device_memory.get(), 0);

  return {std::move(buffer), std::move(device_memory), capacity, {}, {}};
}