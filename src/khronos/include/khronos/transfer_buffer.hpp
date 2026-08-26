#pragma once

#include <cstdint>
#include <memory>

namespace khronos
{
class transfer_buffer
{
  public:
    transfer_buffer(class graphical_device const & graphical_device, std::uint64_t const capacity);

    transfer_buffer(transfer_buffer &&) noexcept = default;

    transfer_buffer(transfer_buffer const &) noexcept = delete;

    transfer_buffer & operator=(transfer_buffer const &) noexcept = delete;

    transfer_buffer & operator=(transfer_buffer &&) noexcept = default;

    ~transfer_buffer() noexcept;

  private:
    friend class transfer_command;
    friend class draw_command;

    std::unique_ptr<struct transfer_buffer_impl> self;
};
}