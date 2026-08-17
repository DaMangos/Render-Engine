#pragma once

#include <khronos/fwd.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <cstddef>
#include <list>
#include <memory>
#include <set>

namespace khronos
{
struct vertex
{
    std::array<float, 2> pos;
    std::array<float, 3> colour;
};

class memory_buffer
{
  public:
    memory_buffer(memory_buffer &&) noexcept = default;

    memory_buffer(memory_buffer const &) noexcept = delete;

    memory_buffer & operator=(memory_buffer &&) noexcept = default;

    memory_buffer & operator=(memory_buffer const &) noexcept = delete;

    ~memory_buffer() = default;

  protected:
    memory_buffer(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
                  std::shared_ptr<vk::raii::Device const> const &         device,
                  vk::DeviceSize const                                    size,
                  vk::BufferUsageFlags2 const                             usage,
                  vk::MemoryPropertyFlags const                           properties);

    vk::DeviceSize                                size;
    std::shared_ptr<vk::raii::Buffer const>       buffer;
    std::shared_ptr<vk::raii::DeviceMemory const> device_memory;
};

class staging_buffer : public memory_buffer
{
  public:
    void copy(std::span<vertex const> const vertices, vertex_transfer_buffer & destination);

    void copy(std::span<std::uint32_t const> const indices, index_transfer_buffer & destination);

  private:
    template <class Type>
    void copy(std::span<Type const> const objects, transfer_buffer & destination);

    friend graphical_device;

    staging_buffer(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
                   std::shared_ptr<vk::raii::Device const> const &         device,
                   std::shared_ptr<vk::raii::Queue const> const &          transfer_queue,
                   std::uint32_t const                                     transfer_queue_family_index,
                   vk::DeviceSize const                                    size);

    std::byte * data;

    struct region
    {
        vk::DeviceSize                                offset;
        vk::DeviceSize                                size;
        std::uint32_t                                 count;
        std::shared_ptr<vk::raii::Buffer const>       destination_buffer;
        std::shared_ptr<vk::raii::DeviceMemory const> destination_device_memory;

        [[nodiscard]]
        bool operator<(region const & other) const noexcept;
    };

    std::set<region> regions;

    std::shared_ptr<vk::raii::CommandPool const> command_pool;

    struct transfer
    {
        std::shared_ptr<vk::raii::CommandBuffer const> command_buffer;
        std::shared_ptr<vk::raii::Fence const>         fence;
    };

    std::list<transfer> transfers;
};

class transfer_buffer : public memory_buffer
{
  protected:
    friend staging_buffer;

    transfer_buffer(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
                    std::shared_ptr<vk::raii::Device const> const &         device,
                    vk::DeviceSize const                                    size,
                    vk::BufferUsageFlags2 const                             usage);
};

class vertex_transfer_buffer : public transfer_buffer
{
  private:
    friend graphical_device;
    friend staging_buffer;

    vertex_transfer_buffer(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
                           std::shared_ptr<vk::raii::Device const> const &         device,
                           vk::DeviceSize const                                    size);

    std::uint32_t vertices_count;
};

class index_transfer_buffer : public transfer_buffer
{
  private:
    friend graphical_device;
    friend staging_buffer;

    index_transfer_buffer(std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device,
                          std::shared_ptr<vk::raii::Device const> const &         device,
                          vk::DeviceSize const                                    size);

    std::uint32_t indices_count;
};
}