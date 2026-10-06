#pragma once

#include <memory>

namespace graphics
{
class compute_pipeline
{
  public:
    explicit compute_pipeline(class graphical_device const & graphical_device);

    compute_pipeline(compute_pipeline &&) noexcept = default;

    compute_pipeline(compute_pipeline const &) noexcept = delete;

    compute_pipeline & operator=(compute_pipeline const &) noexcept = delete;

    compute_pipeline & operator=(compute_pipeline &&) noexcept = default;

    ~compute_pipeline() noexcept;

    void attach(class mesh_buffer const & mesh_buffer);

    void attach(class camera const & camera);

  private:
    std::unique_ptr<struct compute_pipeline_impl> self;
};
}
