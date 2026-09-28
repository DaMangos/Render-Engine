#pragma once

#include <maths/matrix.hpp>

#include <memory>

namespace graphics
{
class uniform_buffers
{
  public:
    uniform_buffers(class graphical_device const & graphical_device, std::size_t const uniform_buffer_count);

    uniform_buffers(uniform_buffers &&) noexcept = default;

    uniform_buffers(uniform_buffers const &) noexcept = delete;

    uniform_buffers & operator=(uniform_buffers const &) noexcept = delete;

    uniform_buffers & operator=(uniform_buffers &&) noexcept = default;

    ~uniform_buffers() noexcept;

    void put(class camera const & camera);

  private:
    friend class transfer_command;

    std::unique_ptr<struct uniform_buffers_impl> self;
};
}