#pragma once

#include <memory>

namespace khronos
{
class transfer_command
{
  public:
    transfer_command(class graphical_device const & graphical_device, std::size_t const command_buffer_count);

    transfer_command(transfer_command &&) noexcept = default;

    transfer_command(transfer_command const &) noexcept = delete;

    transfer_command & operator=(transfer_command const &) noexcept = delete;

    transfer_command & operator=(transfer_command &&) noexcept = default;

    ~transfer_command() noexcept;

    void operator()(class staging_buffer & staging_buffer, class transfer_buffer & transfer_buffer);

  private:
    std::unique_ptr<struct transfer_command_impl> self;
};
}