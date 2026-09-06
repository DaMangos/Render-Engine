#pragma once

#include <array>
#include <memory>

namespace khronos
{
class graphics_pipeline
{
  public:
    struct vertex
    {
        std::array<float, 2> pos;
        std::array<float, 3> colour;
    };

    struct uniform_buffer_object
    {
        std::array<std::array<float, 4>, 4> model;
        std::array<std::array<float, 4>, 4> view;
        std::array<std::array<float, 4>, 4> proj;
    };

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
