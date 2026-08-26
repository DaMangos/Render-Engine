#include "impl.hpp"

namespace khronos
{
[[nodiscard]]
transfer_command_impl create_transfer_command_impl(queue const &       queue,
                                                   std::uint32_t const queue_family_index,
                                                   std::size_t const   command_buffer_count);

[[nodiscard]]
draw_command_impl create_draw_command_impl(queue const &       queue,
                                           std::uint32_t const queue_family_index,
                                           std::size_t const   image_count,
                                           std::size_t const   command_buffer_count);

void wait_for_fence(vk::raii::Device const & device, vk::raii::Fence const & fence);

template <class Type>
Type & get_next(std::list<Type> & list) noexcept
{
  list.splice(list.end(), list, list.begin());

  return list.front();
}
}
