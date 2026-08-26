#include "command.hpp"

#include <khronos/graphical_device.hpp>
#include <khronos/staging_buffer.hpp>
#include <khronos/transfer_buffer.hpp>
#include <khronos/transfer_command.hpp>
#include <logging/logging.hpp>

#include <ranges>
#include <vector>

namespace
{
[[nodiscard]]
static constexpr vk::BufferCopy2 to_buffer_copy_2(vk::StridedDeviceAddressRangeKHR const & region) noexcept
{
  return vk::BufferCopy2{}.setSrcOffset(region.address).setDstOffset(region.address).setSize(region.size);
}

[[nodiscard]]
static std::vector<vk::BufferCopy2> make_regions(std::set<vk::StridedDeviceAddressRangeKHR> const & regions)
{
  return regions | std::views::transform(to_buffer_copy_2) | std::ranges::to<std::vector>();
}
}

khronos::transfer_command::transfer_command(graphical_device const & graphical_device,
                                            std::size_t const        command_buffer_count)
: self(std::make_unique<transfer_command_impl>(create_transfer_command_impl(graphical_device.self->queue,
                                                                            graphical_device.self->queue_family_index,
                                                                            command_buffer_count)))
{
}

khronos::transfer_command::~transfer_command() noexcept = default;

void khronos::transfer_command::operator()(staging_buffer & staging_buffer, transfer_buffer & transfer_buffer)
{
  auto const & device = self->fences.get_dependency<vk::raii::Device>();
  auto const & queue  = self->fences.get_dependency<vk::raii::Queue>();

  auto const & fence          = get_next(self->fences.get());
  auto const & command_buffer = get_next(self->command_buffers.get());

  wait_for_fence(device, fence);

  auto const command_buffer_begin_info = vk::CommandBufferBeginInfo{}  //
                                           .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

  auto const regions = make_regions(staging_buffer.self->occupied_regions);

  auto const copy_buffer_info = vk::CopyBufferInfo2{}
                                  .setDstBuffer(transfer_buffer.self->buffer.get())
                                  .setSrcBuffer(staging_buffer.self->buffer.get())
                                  .setRegions(regions);

  command_buffer.begin(command_buffer_begin_info);
  command_buffer.copyBuffer2(copy_buffer_info);
  command_buffer.end();

  auto const submit_info = vk::SubmitInfo{}.setCommandBuffers(*command_buffer);

  queue.submit(submit_info, *fence);

  transfer_buffer.self->occupied_regions         = std::exchange(staging_buffer.self->occupied_regions, {});
  transfer_buffer.self->usage_to_occupied_region = std::exchange(staging_buffer.self->usage_to_occupied_region, {});
}