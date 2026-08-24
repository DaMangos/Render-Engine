#pragma once

#include <khronos/fwd.hpp>

#include <memory>

namespace khronos
{
class graphical_device
{
  public:
    explicit graphical_device(library const & library);

    void flush(staging_buffer & staging_buffer);

    void draw(graphics_pipeline &      graphics_pipeline,
              index_transfer_buffer &  index_transfer_buffer,
              vertex_transfer_buffer & vertex_transfer_buffer,
              render_window &          render_window) const;

  private:
    std::unique_ptr<struct graphical_device_impl, void (*)(graphical_device_impl *)> ptr;
};
}