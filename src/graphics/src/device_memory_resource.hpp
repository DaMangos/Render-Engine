#pragma once

#include "vulakn_handles.hpp"

#include <unordered_map>

namespace graphics
{
class device_memory_resource : public std::pmr::memory_resource
{
  public:
    device_memory_resource(vulkan::buffer::dependencies_type const & buffer_dependencies, vk::BufferUsageFlags const usage);

    [[nodiscard]]
    vulkan::buffer::dependencies_type get_buffer_dependencies() const noexcept;

    [[nodiscard]]
    vulkan::buffer * get_buffer(void * const ptr) noexcept;

    [[nodiscard]]
    vulkan::buffer const * get_buffer(void * const ptr) const noexcept;

    [[nodiscard]]
    vulkan::device_memory * get_device_memory(void * const ptr) noexcept;

    [[nodiscard]]
    vulkan::device_memory const * get_device_memory(void * const ptr) const noexcept;

  private:
    [[nodiscard]]
    void * do_allocate(std::size_t const bytes, std::size_t const alignment) override;

    void do_deallocate(void * const ptr, std::size_t bytes, std::size_t alignment) override;

    [[nodiscard]]
    bool do_is_equal(std::pmr::memory_resource const & other) const noexcept override;

    vk::BufferUsageFlags                                                          usage;
    vulkan::buffer::dependencies_type                                             buffer_dependencies;
    std::unordered_map<void *, std::tuple<vulkan::buffer, vulkan::device_memory>> heap;
};
}