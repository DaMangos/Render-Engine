#include "buffer.hpp"
#include "impl.hpp"
#include "khronos/graphics_pipeline.hpp"

#include <khronos/graphical_device.hpp>
#include <khronos/staging_buffer.hpp>

#include <cstring>
#include <optional>
#include <stdexcept>

namespace
{
template <class Type>
[[nodiscard]]
static constexpr vk::DeviceSize align_up(vk::DeviceSize const offset) noexcept
{
  auto const remainder = offset % alignof(Type);
  return remainder == 0 ? offset : offset - remainder + alignof(Type);
};

template <class Type>
[[nodiscard]]
static constexpr std::optional<vk::StridedDeviceAddressRangeKHR> find_available_region(
  std::span<Type const> const       objects,
  vk::DeviceSize const              capacity,
  khronos::occupied_regions const & occupied_regions) noexcept
{
  if(objects.size() > std::numeric_limits<std::uint32_t>::max())
    return std::nullopt;

  auto region = vk::StridedDeviceAddressRangeKHR{}  //
                  .setAddress(0)                    //
                  .setSize(objects.size_bytes())    //
                  .setStride(sizeof(Type));

  for(auto const & occupied_region : occupied_regions)
  {
    if(region.address + region.size <= occupied_region.address)
      return region;

    region.address = align_up<Type>(occupied_region.address + occupied_region.size);
  }

  return region.address + region.size <= capacity ? std::make_optional(region) : std::nullopt;
}

}

khronos::staging_buffer::staging_buffer(graphical_device const & graphical_device, std::uint64_t const size)
: self(std::make_unique<staging_buffer_impl>(create_staging_buffer_impl(graphical_device.self->physical_device,
                                                                        graphical_device.self->device,
                                                                        static_cast<vk::DeviceSize>(size))))
{
}

khronos::staging_buffer::~staging_buffer() noexcept
{
  if(self)
    self->device_memory.get().unmapMemory();
}

void khronos::staging_buffer::memcpy(std::span<graphics_pipeline::vertex const> const vertices)
{
  if(self->usage_to_occupied_region.contains(vk::BufferUsageFlagBits::eVertexBuffer))
    throw std::logic_error("vertex buffer has already been allocated");

  auto available_region = find_available_region(vertices, self->capacity, self->occupied_regions);

  if(not available_region)
    throw;

  auto const & [available_region_iterator, _] = self->occupied_regions.emplace(*available_region);

  self->usage_to_occupied_region.emplace(vk::BufferUsageFlagBits::eVertexBuffer, available_region_iterator);

  std::memcpy(self->mapped_memory + available_region->address, vertices.data(), vertices.size_bytes());
}

void khronos::staging_buffer::memcpy(std::span<std::uint32_t const> const indices)
{
  if(self->usage_to_occupied_region.contains(vk::BufferUsageFlagBits::eIndexBuffer))
    throw std::logic_error("index buffer has already been allocated");

  auto available_region = find_available_region(indices, self->capacity, self->occupied_regions);

  if(not available_region)
    throw;

  auto const & [available_region_iterator, _] = self->occupied_regions.emplace(*available_region);

  self->usage_to_occupied_region.emplace(vk::BufferUsageFlagBits::eIndexBuffer, available_region_iterator);

  std::memcpy(self->mapped_memory + available_region->address, indices.data(), indices.size_bytes());
}