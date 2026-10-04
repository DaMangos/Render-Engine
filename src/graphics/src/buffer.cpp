#include "buffer.hpp"

#include "dependencies.hpp"
#include "vulakn_handles.hpp"

#include <logging/logging.hpp>

#include <algorithm>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>

namespace
{
[[nodiscard]]
static std::optional<std::uint32_t> find_memory_type_index(vk::MemoryRequirements const &             memory_requirements,
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
static graphics::vulkan::buffer create_buffer(graphics::vulkan::buffer::dependencies_type const & buffer_dependencies,
                                              vk::DeviceSize const                                allocation_size,
                                              vk::BufferUsageFlags const                          usage)
{
  auto const buffer_create_info = vk::BufferCreateInfo{}       //
                                    .setSize(allocation_size)  //
                                    .setUsage(usage)           //
                                    .setSharingMode(vk::SharingMode::eExclusive);

  return {buffer_dependencies, buffer_dependencies.get<vk::raii::Device>(), buffer_create_info};
}

[[nodiscard]]
static std::pair<graphics::vulkan::device_memory, std::span<std::byte>> allocate_host_visible_device_memory(
  graphics::vulkan::device_memory::dependencies_type const & device_memory_dependencies)
{
  auto const memory_properties = device_memory_dependencies.get<vk::raii::PhysicalDevice>().getMemoryProperties();

  auto const memory_requirements = device_memory_dependencies.get<vk::raii::Buffer>().getMemoryRequirements();

  constexpr auto memory_property = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;

  auto const memory_type_index = find_memory_type_index(memory_requirements, memory_properties, memory_property);

  if(not memory_type_index)
    throw std::bad_alloc();

  auto const memory_allocate_info = vk::MemoryAllocateInfo{}                   //
                                      .setMemoryTypeIndex(*memory_type_index)  //
                                      .setAllocationSize(memory_requirements.size);

  auto device_memory = graphics::vulkan::device_memory{[](vk::raii::DeviceMemory const & device_memory)
                                                       { device_memory.unmapMemory(); },
                                                       device_memory_dependencies,
                                                       device_memory_dependencies.get<vk::raii::Device>(),
                                                       memory_allocate_info};

  device_memory_dependencies.get<vk::raii::Buffer>().bindMemory(device_memory.get(), 0);

  std::byte * const mapped_memory = static_cast<std::byte *>(device_memory.get().mapMemory(0, vk::WholeSize));

  return {
    std::move(device_memory),
    {mapped_memory, memory_requirements.size}
  };
}

[[nodiscard]]
static graphics::vulkan::multi_purpose_device_memory allocate_device_local_device_memory(
  graphics::vulkan::multi_purpose_device_memory::dependencies_type const & multi_purpose_device_memory_dependencies)

{
  auto const memory_properties = multi_purpose_device_memory_dependencies.get<vk::raii::PhysicalDevice>().getMemoryProperties();

  auto const & buffers = multi_purpose_device_memory_dependencies.get<std::vector<vk::raii::Buffer>>();

  assert(not buffers.empty());

  constexpr auto to_memory_requirements = [](vk::raii::Buffer const & buffer)
  {
    return buffer.getMemoryRequirements();
  };

  auto const memory_requirements = buffers | std::views::transform(to_memory_requirements) | std::ranges::to<std::vector>();

  auto const to_memory_type_index = [&memory_properties](vk::MemoryRequirements const & memory_requirements)
  {
    constexpr auto memory_property = vk::MemoryPropertyFlagBits::eDeviceLocal;

    auto const index = find_memory_type_index(memory_requirements, memory_properties, memory_property);

    return index ? *index : throw std::runtime_error("failed to find memory type index for " + vk::to_string(memory_property));
  };

  auto const memory_type_indices = memory_requirements | std::views::transform(to_memory_type_index);

  if(std::ranges::adjacent_find(memory_type_indices, std::ranges::not_equal_to()) != memory_type_indices.end())
    throw;

  constexpr auto accumulate_allocation_size = [](vk::DeviceSize const size, vk::MemoryRequirements const & memory_requirements)
  {
    return size + memory_requirements.size;
  };

  auto const allocation_size = std::accumulate(memory_requirements.begin(),
                                               memory_requirements.end(),
                                               vk::DeviceSize{},
                                               accumulate_allocation_size);

  auto const memory_allocate_info = vk::MemoryAllocateInfo{}                            //
                                      .setMemoryTypeIndex(memory_type_indices.front())  //
                                      .setAllocationSize(allocation_size);

  auto device_memory = graphics::vulkan::multi_purpose_device_memory{
    multi_purpose_device_memory_dependencies,
    multi_purpose_device_memory_dependencies.get<vk::raii::Device>(),
    memory_allocate_info};

  auto const bind_memory = [&device_memory](vk::DeviceSize const size, vk::raii::Buffer const & buffer)
  {
    auto const memory_requirements = buffer.getMemoryRequirements();

    buffer.bindMemory(device_memory.get(), size);

    return size + memory_requirements.size;
  };

  std::accumulate(buffers.begin(), buffers.end(), vk::DeviceSize{}, bind_memory);

  return device_memory;
}

[[nodiscard]]
static graphics::staging_buffer::impl make_staging_buffer_impl(
  graphics::vulkan::buffer::dependencies_type const & buffer_dependencies,
  vk::DeviceSize const                                allocation_size)
{
  auto buffer = create_buffer(buffer_dependencies, allocation_size, vk::BufferUsageFlagBits::eTransferSrc);

  auto [device_memory, mapped_memory] = allocate_host_visible_device_memory(buffer.as_dependencies());

  return {std::move(buffer), std::move(device_memory), mapped_memory};
}

[[nodiscard]]
static graphics::uniform_buffer::impl make_uniform_buffer_impl(
  graphics::vulkan::buffer::dependencies_type const & buffer_dependencies,
  vk::DeviceSize const                                allocation_size)
{
  auto buffer = create_buffer(buffer_dependencies, allocation_size, vk::BufferUsageFlagBits::eUniformBuffer);

  auto [device_memory, mapped_memory] = allocate_host_visible_device_memory(buffer.as_dependencies());

  return {std::move(buffer), std::move(device_memory), mapped_memory};
}

[[nodiscard]]
static graphics::transfer_buffer::impl make_transfer_buffer_impl(
  graphics::vulkan::buffers::dependencies_type const & buffers_dependencies,
  std::span<vk::DeviceSize const> const                allocation_sizes)
{
  auto buffers = graphics::vulkan::buffers{buffers_dependencies};

  for(auto const allocation_size : allocation_sizes)
  {
    auto const buffer_create_info = vk::BufferCreateInfo{}
                                      .setSize(allocation_size)
                                      .setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer)
                                      .setSharingMode(vk::SharingMode::eExclusive);

    buffers.get().emplace_back(buffers.get_dependency<vk::raii::Device>(), buffer_create_info);
  }

  return {std::move(buffers), allocate_device_local_device_memory(buffers.as_dependencies())};
}
}

graphics::staging_buffer::staging_buffer(graphics::vulkan::buffer::dependencies_type const & buffer_dependencies,
                                         vk::DeviceSize const                                allocation_size)
: self(make_staging_buffer_impl(buffer_dependencies, allocation_size))
{
}

graphics::vulkan::buffer const & graphics::staging_buffer::get_buffer() const noexcept
{
  return self.buffer;
}

graphics::vulkan::device_memory const & graphics::staging_buffer::get_device_memory() const noexcept
{
  return self.device_memory;
}

std::span<std::byte> graphics::staging_buffer::get_mapped_memory() const noexcept
{
  return self.mapped_memory;
}

graphics::uniform_buffer::uniform_buffer(graphics::vulkan::buffer::dependencies_type const & buffer_dependencies,
                                         vk::DeviceSize const                                allocation_size)
: self(make_uniform_buffer_impl(buffer_dependencies, allocation_size))
{
}

graphics::vulkan::buffer const & graphics::uniform_buffer::get_buffer() const noexcept
{
  return self.buffer;
}

graphics::vulkan::device_memory const & graphics::uniform_buffer::get_device_memory() const noexcept
{
  return self.device_memory;
}

std::span<std::byte> graphics::uniform_buffer::get_mapped_memory() const noexcept
{
  return self.mapped_memory;
}

graphics::transfer_buffer::transfer_buffer(graphics::vulkan::buffers::dependencies_type const & buffers_dependencies,
                                           std::span<vk::DeviceSize const> const                allocation_sizes)
: self(make_transfer_buffer_impl(buffers_dependencies, allocation_sizes))
{
}

graphics::vulkan::buffers const & graphics::transfer_buffer::get_buffers() const noexcept
{
  return self.buffers;
}

graphics::vulkan::multi_purpose_device_memory const & graphics::transfer_buffer::get_device_memory() const noexcept
{
  return self.device_memory;
}