#include "impl.hpp"

namespace khronos
{
[[nodiscard]]
staging_buffer_impl create_staging_buffer_impl(physical_device const & physical_device,
                                               device const &          device,
                                               vk::DeviceSize const    capacity);

[[nodiscard]]
transfer_buffer_impl create_transfer_buffer_impl(physical_device const & physical_device,
                                                 device const &          device,
                                                 vk::DeviceSize const    capacity);
}