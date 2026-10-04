#pragma once

#include "vulakn_handles.hpp"

#include <unordered_map>

namespace graphics
{
template <class HostVisibleBuffer>
class device_memory_resource : public std::pmr::memory_resource
{
  public:
    using host_visible_buffer_type = HostVisibleBuffer;

    device_memory_resource(vulkan::buffer::dependencies_type const & buffer_dependencies) noexcept;

    [[nodiscard]]
    vulkan::buffer::dependencies_type const & get_buffer_dependencies() const noexcept
    {
      return buffer_dependencies;
    }

    [[nodiscard]]
    host_visible_buffer_type find_host_visible_buffer(void const * const ptr) const
    {
      /*

      To figure out if a ptr is pointing to a memory address within a span we cannot naively do,

        return span.data() <= ptr and ptr < span.data() + span.size();

      This results in undefined behaviour when the ptr and the span points to different memory allocations.
      To avoid undefined behaviour, we can use the total ordering guaranteed by std::less, like this,

        return std::greater_equal<>{}(span.data(), ptr) and std::less<>{}(ptr, span.data() + span.size());

      However, this can produce false positives in the case of interleaved values. For example,

        T a[3];
        T b[3];

      std::less might order the two arrays like,

        &a[0] < &b[0] < &a[1] < &b[1] < &a[2] < &b[2]

      This is a valid total ordering but will produce the result that &b[1] ∈ [&a[0], &a[2]), which is incorrect.
      Therefore, the only valid way of deducing if a ptr is pointing to a memory address within a span is by checking
      if each byte's memory address is equivent the the given ptr. Thus, an idiomatic way of doing this is,

        return std::ranges::any_of(span, [ptr](std::byte const & byte) { return &byte == ptr; });

      Unfortunately, this is quite inefficient as we have to check every single byte in the span. However, until
      this paper is accepted into the standard libary we have no other choice.

        https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3234r1.html

      */

      for(auto const & [_, host_visible_buffer] : host_visible_buffers)
        // No execution policy as it perfroms slower, see src/graphics/benchmark/benchmark_pointer_in_range.cpp
        if(std::ranges::any_of(host_visible_buffer.get_mapped_memory(), [ptr](std::byte const & byte) { return &byte == ptr; }))
          return host_visible_buffer;

      throw std::out_of_range("could not find allocation region");
    }

    [[nodiscard]]
    host_visible_buffer_type get_buffer(void const * const ptr) const
    {
      return host_visible_buffers.at(ptr);
    }

  protected:
    [[nodiscard]]
    void * do_allocate(std::size_t const size, std::size_t) override
    {
      auto const allocation_region = host_visible_buffer_type{buffer_dependencies, size};

      host_visible_buffers.emplace(allocation_region.get_mapped_memory().data(), allocation_region);

      return allocation_region.get_mapped_memory().data();
    }

    void do_deallocate(void * const ptr, std::size_t, std::size_t) override
    {
      host_visible_buffers.erase(ptr);
    }

    [[nodiscard]]
    bool do_is_equal(std::pmr::memory_resource const & other) const noexcept override
    {
      return this == &other;
    }

  private:
    vulkan::buffer::dependencies_type                          buffer_dependencies;
    std::unordered_map<void const *, host_visible_buffer_type> host_visible_buffers;
};
}