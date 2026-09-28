#pragma once

#include "device_memory_resource.hpp"

#include "find_memory_type_index.hpp"

#include <new>

graphics::device_memory_resource::device_memory_resource(vulkan::buffer::dependencies_type const & buffer_dependencies,
                                                         vk::BufferUsageFlags const                usage)
: usage(usage),
  buffer_dependencies(buffer_dependencies)
{
}

graphics::vulkan::buffer::dependencies_type graphics::device_memory_resource::get_buffer_dependencies() const noexcept
{
  return buffer_dependencies;
}

graphics::vulkan::buffer * graphics::device_memory_resource::get_buffer(void * const ptr) noexcept
{
  auto it = heap.find(ptr);

  return it == heap.end() ? nullptr : &std::get<vulkan::buffer>(it->second);
}

graphics::vulkan::buffer const * graphics::device_memory_resource::get_buffer(void * const ptr) const noexcept
{
  auto it = heap.find(ptr);

  return it == heap.end() ? nullptr : &std::get<vulkan::buffer>(it->second);
}

graphics::vulkan::device_memory * graphics::device_memory_resource::get_device_memory(void * const ptr) noexcept
{
  auto it = heap.find(ptr);

  return it == heap.end() ? nullptr : &std::get<vulkan::device_memory>(it->second);
}

graphics::vulkan::device_memory const * graphics::device_memory_resource::get_device_memory(void * const ptr) const noexcept
{
  auto it = heap.find(ptr);

  return it == heap.end() ? nullptr : &std::get<vulkan::device_memory>(it->second);
}

void * graphics::device_memory_resource::do_allocate(std::size_t const bytes, std::size_t const alignment)
{
  auto const buffer_create_info = vk::BufferCreateInfo{}  //
                                    .setSize(bytes)       //
                                    .setUsage(usage)      //
                                    .setSharingMode(vk::SharingMode::eExclusive);

  auto buffer = graphics::vulkan::buffer{buffer_dependencies,
                                         buffer_dependencies.template get<vk::raii::Device>(),
                                         buffer_create_info};

  auto const memory_properties = buffer_dependencies.template get<vk::raii::PhysicalDevice>().getMemoryProperties();

  auto const memory_requirements = buffer.get().getMemoryRequirements();

  if(alignment % memory_requirements.alignment != 0)
    throw std::bad_alloc();

  auto const memory_property = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

  auto const memory_type_index = find_memory_type_index(memory_requirements, memory_properties, memory_property);

  if(not memory_type_index)
    throw std::bad_alloc();

  auto const memory_allocate_info = vk::MemoryAllocateInfo{}                   //
                                      .setMemoryTypeIndex(*memory_type_index)  //
                                      .setAllocationSize(memory_requirements.size);

  auto device_memory = graphics::vulkan::device_memory{[](vk::raii::DeviceMemory const & device_memory)
                                                       { device_memory.unmapMemory(); },
                                                       buffer.as_dependencies(),
                                                       buffer.as_dependencies().template get<vk::raii::Device>(),
                                                       memory_allocate_info};

  buffer.get().bindMemory(device_memory.get(), 0);

  void * ptr = device_memory.get().mapMemory(0, vk::WholeSize);

  heap.emplace(ptr, std::forward_as_tuple(std::move(buffer), std::move(device_memory)));

  return ptr;
}

void graphics::device_memory_resource::do_deallocate(void * const ptr, std::size_t, std::size_t)
{
  heap.erase(ptr);
}

bool graphics::device_memory_resource::do_is_equal(std::pmr::memory_resource const & other) const noexcept
{
  auto * const other_ptr = dynamic_cast<device_memory_resource const *>(&other);

  return other_ptr and heap == other_ptr->heap;
}
