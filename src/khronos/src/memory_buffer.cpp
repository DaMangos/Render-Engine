#include "detail/make_shared_with_data.hpp"
#include "vulkan/vulkan.hpp"

#include <khronos/memory_buffer.hpp>
#include <logging/logging.hpp>
#include <logging/serialize.hpp>

#include <cstddef>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <ranges>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

khronos::memory_buffer::memory_buffer(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
                                      std::shared_ptr<vk::raii::Device const> const &         device,
                                      vk::DeviceSize const                                    size,
                                      vk::BufferUsageFlags2 const                             usage,
                                      vk::MemoryPropertyFlags const                           properties)
{
  auto const memory_properties = physical_device->getMemoryProperties();

  auto const [buffer_create_info, _] = vk::StructureChain{vk::BufferCreateInfo{}  //
                                                            .setSize(size)
                                                            .setSharingMode(vk::SharingMode::eExclusive),
                                                          vk::BufferUsageFlags2CreateInfoKHR{}.setUsage(usage)};

  buffer = detail::make_shared_with_data<vk::raii::Buffer const>(*device, buffer_create_info);

  detail::emplace_data(buffer, device);

  auto const memory_requirements = buffer->getMemoryRequirements();

  std::uint32_t memory_type_index = 0;
  for(; memory_type_index < memory_properties.memoryTypeCount; memory_type_index++)
    if(memory_requirements.memoryTypeBits & (1u << memory_type_index)
       and (memory_properties.memoryTypes[memory_type_index].propertyFlags & properties) == properties)
      break;

  if(memory_type_index == memory_properties.memoryTypeCount)
    throw std::runtime_error("cannot find a memory properties " + vk::to_string(properties) + " for usage "
                             + vk::to_string(usage));

  auto const memory_allocate_info = vk::MemoryAllocateInfo{}  //
                                      .setMemoryTypeIndex(memory_type_index)
                                      .setAllocationSize(memory_requirements.size);

  device_memory = detail::make_shared_with_data<vk::raii::DeviceMemory const>(*device, memory_allocate_info);

  detail::emplace_data(device_memory, buffer);

  capacity = size;

  buffer->bindMemory(*device_memory, 0);
}

void khronos::staging_buffer::copy(std::span<vertex const> const vertices, vertex_transfer_buffer & destination)
{
  destination.vertices_count = static_cast<std::uint32_t>(vertices.size());

  copy<>(vertices, destination);
}

void khronos::staging_buffer::copy(std::span<std::uint32_t const> const indices, index_transfer_buffer & destination)
{
  destination.indices_count = static_cast<std::uint32_t>(indices.size());

  copy<>(indices, destination);
}

template <class Type>
void khronos::staging_buffer::copy(std::span<Type const> const objects, transfer_buffer & destination)
{
  using namespace logging::serialize;

  if(objects.size_bytes() > destination.capacity)
    throw vk::OutOfHostMemoryError{(std::stringstream{} << "unable to allocate a region for " << objects.size_bytes()
                                                        << " bytes because the destination buffer has a capacity of "
                                                        << destination.capacity << " bytes")
                                     .str()};

  auto const align_up = [&](vk::DeviceSize const offset)
  {
    auto const remainder = offset % alignof(Type);
    return remainder == 0 ? offset : offset - remainder + alignof(Type);
  };

  if(objects.size() > std::numeric_limits<std::uint32_t>::max())
    throw vk::OutOfHostMemoryError{
      (std::stringstream{} << "unable to allocate a region for " << objects.size() << " objects ").str()};

  region new_region = {
    .offset                    = 0,
    .size                      = objects.size_bytes(),
    .count                     = static_cast<std::uint32_t>(objects.size()),
    .destination_buffer        = destination.buffer,
    .destination_device_memory = destination.device_memory,
  };

  for(auto next_region = regions.begin(); next_region != regions.end(); ++next_region)
  {
    if(new_region.offset + new_region.size <= next_region->offset)
    {
      regions.emplace_hint(std::ranges::next(next_region), new_region);

      std::memcpy(static_cast<std::byte *>(data) + new_region.offset, objects.data(), objects.size_bytes());

      logging::verbose() << " found a new region at offset " << new_region.offset << " for size " << new_region.size;

      return;
    }

    new_region.offset = align_up(next_region->offset + next_region->size);
  }

  if(new_region.offset + new_region.size <= capacity)
  {
    regions.emplace_hint(regions.end(), new_region);

    std::memcpy(static_cast<std::byte *>(data) + new_region.offset, objects.data(), objects.size_bytes());

    logging::verbose() << " found a new region at offset " << new_region.offset << " for size " << new_region.size;

    return;
  }

  auto const to_pair = [](region const & region)
  {
    return std::tuple{region.offset, region.offset + region.size};
  };

  auto memory = regions | std::views::transform(to_pair) | std::ranges::to<std::vector>();

  throw vk::OutOfHostMemoryError{(std::stringstream{} << "unable to allocate a region of " << objects.size_bytes()
                                                      << " bytes because device memory " << std::pair{0, capacity}
                                                      << " is already segmented into " << memory)
                                   .str()};
}

khronos::staging_buffer::staging_buffer(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
                                        std::shared_ptr<vk::raii::Device const> const &         device,
                                        std::shared_ptr<vk::raii::Queue const> const &          transfer_queue,
                                        std::uint32_t const  transfer_queue_family_index,
                                        vk::DeviceSize const size)
: memory_buffer(physical_device,
                device,
                size,
                vk::BufferUsageFlagBits2::eTransferSrc,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent),
  data(device_memory->mapMemory(0ull, size))
{
  auto const command_pool_create_info = vk::CommandPoolCreateInfo{}
                                          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                                          .setQueueFamilyIndex(transfer_queue_family_index);

  command_pool = detail::make_shared_with_data<vk::raii::CommandPool const>(*device, command_pool_create_info);

  auto const command_buffer_allocate_info = vk::CommandBufferAllocateInfo{}
                                              .setCommandPool(*command_pool)
                                              .setLevel(vk::CommandBufferLevel::ePrimary)
                                              .setCommandBufferCount(1);

  auto const create_shared_command_buffer = [&](vk::raii::CommandBuffer & command_buffer)
  {
    auto const shared_command_buffer
      = detail::make_shared_with_data<vk::raii::CommandBuffer const>(std::move(command_buffer));

    detail::emplace_data(shared_command_buffer, command_pool);

    return shared_command_buffer;
  };

  for(auto && command_buffer : vk::raii::CommandBuffers(*device, command_buffer_allocate_info)
                                 | std::views::transform(create_shared_command_buffer))
  {
    auto const fence_create_info = vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled);

    auto const fence = detail::make_shared_with_data<vk::raii::Fence const>(*device, fence_create_info);

    detail::emplace_data(fence, device);
    detail::emplace_data(transfer_queue, fence);

    auto const wait_for_fence = [=](vk::raii::CommandBuffer const &)
    {
      auto const wait_for_fences_result
        = device->waitForFences(**fence, vk::True, std::numeric_limits<std::uint64_t>::max());

      if(wait_for_fences_result < vk::Result::eSuccess)
        logging::error() << "wait for fences returned a error: " << vk::to_string(wait_for_fences_result);

      if(wait_for_fences_result > vk::Result::eSuccess)
        logging::warning() << "wait for fences returned a warring: " << vk::to_string(wait_for_fences_result);
    };

    detail::emplace_function(command_buffer, wait_for_fence);

    transfers.emplace_back(command_buffer, fence);
  }
}

bool khronos::staging_buffer::staging_buffer::region::operator<(region const & other) const noexcept
{
  return offset < other.offset;
}

khronos::transfer_buffer::transfer_buffer(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
                                          std::shared_ptr<vk::raii::Device const> const &         device,
                                          vk::DeviceSize const                                    size,
                                          vk::BufferUsageFlags2 const                             usage)
: memory_buffer(physical_device, device, size, usage, vk::MemoryPropertyFlagBits::eDeviceLocal)
{
}

khronos::vertex_transfer_buffer::vertex_transfer_buffer(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
  std::shared_ptr<vk::raii::Device const> const &         device,
  vk::DeviceSize const                                    size)
: transfer_buffer(physical_device,
                  device,
                  size,
                  vk::BufferUsageFlagBits2::eVertexBuffer | vk::BufferUsageFlagBits2::eTransferDst)
{
}

khronos::index_transfer_buffer::index_transfer_buffer(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
  std::shared_ptr<vk::raii::Device const> const &         device,
  vk::DeviceSize const                                    size)
: transfer_buffer(physical_device,
                  device,
                  size,
                  vk::BufferUsageFlagBits2::eIndexBuffer | vk::BufferUsageFlagBits2::eTransferDst)
{
}
