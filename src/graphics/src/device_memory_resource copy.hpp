#pragma once

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <memory_resource>
#include <ranges>

namespace graphics
{
template <class Buffer>
class device_memory_resource : public std::pmr::memory_resource
{
  public:
    template <class... Args>
    device_memory_resource(Args &&... args)
    : emplace_front([=](std::list<Buffer> & buffers, std::size_t const size, std::size_t const alignment)
                    { buffers.emplace_front(std::forward<Args>(args)..., size, alignment); })
    {
    }

    device_memory_resource(device_memory_resource &&) = delete;

    device_memory_resource(device_memory_resource const &) = delete;

    device_memory_resource & operator=(device_memory_resource &&) = delete;

    device_memory_resource & operator=(device_memory_resource const &) = delete;

    ~device_memory_resource() noexcept = default;

    [[nodiscard]]
    bool empty() const noexcept
    {
      return buffers.empty() and begin_ptrs.empty() and end_ptrs.empty();
    }

    [[nodiscard]]
    Buffer & find_buffer(void const * const ptr)
    {
      constexpr auto intersection = [](std::ranges::range auto const & lhs, std::ranges::range auto const & rhs, auto func)
      {
        for(auto const & x : lhs)
          for(auto const & y : rhs)
            if(x == y and func(x))
              return x;
        throw std::out_of_range("cannot find buffer");
      };

      auto const is_pointer_in_buffer = [ptr](typename std::list<Buffer>::iterator const buffer)
      {
        // No need for a execution policy, see benchmark/benchmark_pointer_in_range.cpp
        return std::ranges::any_of(buffer->get_mapped_memory(), [ptr](std::byte const & byte) { return &byte == ptr; });
      };

      auto const lower_half_candidate_buffers = std::ranges::subrange(begin_ptrs.begin(),
                                                                      begin_ptrs.upper_bound(static_cast<std::byte const *>(ptr)))
                                              | std::views::values;

      auto const upper_half_candidate_buffers = std::ranges::subrange(end_ptrs.upper_bound(static_cast<std::byte const *>(ptr)),
                                                                      end_ptrs.end())
                                              | std::views::values;

      return *intersection(lower_half_candidate_buffers, upper_half_candidate_buffers, is_pointer_in_buffer);
    }

  protected:
    [[nodiscard]]
    void * do_allocate(std::size_t const size, std::size_t const alignment) override
    {
      emplace_front(buffers, size, alignment);

      auto host_visible_buffer = buffers.begin();

      std::byte * const ptr = host_visible_buffer->get_mapped_memory().data();

      begin_ptrs.emplace(ptr, host_visible_buffer);
      end_ptrs.emplace(ptr + size, host_visible_buffer);

      return ptr;
    }

    void do_deallocate(void * const ptr, std::size_t size, std::size_t alignment [[maybe_unused]]) override
    {
      buffers.erase(begin_ptrs.at(static_cast<std::byte const *>(ptr)));
      begin_ptrs.erase(static_cast<std::byte const *>(ptr));
      end_ptrs.erase(static_cast<std::byte const *>(ptr) + size);
    }

    [[nodiscard]]
    bool do_is_equal(std::pmr::memory_resource const & other) const noexcept override
    {
      return this == &other;
    }

  private:
    std::function<void(std::list<Buffer> &, std::size_t const, std::size_t const)> emplace_front;
    std::list<Buffer>                                                              buffers;

    std::map<std::byte const *, typename std::list<Buffer>::iterator> begin_ptrs;
    std::map<std::byte const *, typename std::list<Buffer>::iterator> end_ptrs;
};
}