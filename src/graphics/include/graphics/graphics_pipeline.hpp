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

  private:
    friend class draw_command;

    std::unique_ptr<struct graphics_pipeline_impl> self;
};
}
