#pragma once

#include <memory>

namespace graphics
{
class graphics_pipeline
{
  public:
    explicit graphics_pipeline(class graphical_device const & graphical_device);

    graphics_pipeline(graphics_pipeline &&) noexcept = default;

    graphics_pipeline(graphics_pipeline const &) noexcept = delete;

    graphics_pipeline & operator=(graphics_pipeline const &) noexcept = delete;

    graphics_pipeline & operator=(graphics_pipeline &&) noexcept = default;

    ~graphics_pipeline() noexcept;

    void attach(class mesh_buffer const & mesh_buffer);

    void attach(class camera const & camera);

    void draw(class render_window & render_window);

  private:
    std::unique_ptr<struct graphics_pipeline_impl> self;
};
}
