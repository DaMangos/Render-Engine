#include "buffer.hpp"

#include <khronos/graphical_device.hpp>
#include <khronos/transfer_buffer.hpp>

khronos::transfer_buffer::transfer_buffer(graphical_device const & graphical_device, std::uint64_t const size)
: self(std::make_unique<transfer_buffer_impl>(create_transfer_buffer_impl(graphical_device.self->physical_device,
                                                                          graphical_device.self->device,
                                                                          static_cast<vk::DeviceSize>(size))))
{
}

khronos::transfer_buffer::~transfer_buffer() noexcept = default;
