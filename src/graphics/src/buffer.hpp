#pragma once

#include "vulakn_handles.hpp"

namespace graphics
{
class staging_buffer
{
  public:
    staging_buffer(graphics::vulkan::buffer::dependencies_type const & buffer_dependencies, vk::DeviceSize const allocation_size);

    [[nodiscard]]
    graphics::vulkan::buffer const & get_buffer() const noexcept;

    [[nodiscard]]
    graphics::vulkan::device_memory const & get_device_memory() const noexcept;

    [[nodiscard]]
    std::span<std::byte> get_mapped_memory() const noexcept;

    struct impl
    {
        graphics::vulkan::buffer        buffer;
        graphics::vulkan::device_memory device_memory;
        std::span<std::byte>            mapped_memory;
    };

  private:
    impl self;
};

class uniform_buffer
{
  public:
    uniform_buffer(graphics::vulkan::buffer::dependencies_type const & buffer_dependencies, vk::DeviceSize const allocation_size);

    [[nodiscard]]
    graphics::vulkan::buffer const & get_buffer() const noexcept;

    [[nodiscard]]
    graphics::vulkan::device_memory const & get_device_memory() const noexcept;

    [[nodiscard]]
    std::span<std::byte> get_mapped_memory() const noexcept;

    struct impl
    {
        graphics::vulkan::buffer        buffer;
        graphics::vulkan::device_memory device_memory;
        std::span<std::byte>            mapped_memory;
    };

  private:
    impl self;
};

class transfer_buffer
{
  public:
    transfer_buffer(graphics::vulkan::buffers::dependencies_type const & buffers_dependencies,
                    std::span<vk::DeviceSize const> const                allocation_sizes);

    [[nodiscard]]
    graphics::vulkan::buffers const & get_buffers() const noexcept;

    [[nodiscard]]
    graphics::vulkan::multi_purpose_device_memory const & get_device_memory() const noexcept;

    struct impl
    {
        graphics::vulkan::buffers                     buffers;
        graphics::vulkan::multi_purpose_device_memory device_memory;
    };

  private:
    impl self;
};
}