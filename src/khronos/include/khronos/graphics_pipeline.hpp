#pragma once

#include <maths/matrix.hpp>

#include <memory>

namespace khronos
{
class graphics_pipeline
{
  public:
    struct vertex
    {
        maths::column_major::float3 pos;
        maths::column_major::float3 colour;
    };

    struct uniform_buffer_object
    {
        maths::column_major::float4x4 model;
        maths::column_major::float4x4 view;
        maths::column_major::float4x4 proj;
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
