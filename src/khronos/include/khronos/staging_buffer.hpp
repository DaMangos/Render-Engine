#pragma once

#include <khronos/graphics_pipeline.hpp>

#include <cstdint>
#include <memory>
#include <span>

namespace khronos
{
class staging_buffer
{
  public:
    staging_buffer(class graphical_device const & graphical_device, std::uint64_t const capacity);

    staging_buffer(staging_buffer &&) noexcept = default;

    staging_buffer(staging_buffer const &) noexcept = delete;

    staging_buffer & operator=(staging_buffer const &) noexcept = delete;

    staging_buffer & operator=(staging_buffer &&) noexcept = default;

    ~staging_buffer() noexcept;

    void memcpy(std::span<graphics_pipeline::vertex const> const vertices);

    void memcpy(std::span<std::uint32_t const> const indices);

  private:
    friend class transfer_command;

    std::unique_ptr<struct staging_buffer_impl> self;
};
}