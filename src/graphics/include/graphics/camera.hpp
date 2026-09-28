#pragma once

#include <maths/coordinates.hpp>
#include <maths/matrix.hpp>

namespace graphics
{
class camera
{
  public:
    void set_pitch(float const pitch) const;

    void set_yaw(float const yaw) const;

    void set_roll(float const roll) const;

    void set_pos(maths::cartesian_coordinate<float, 3> const pos) const noexcept;

    void look_at(maths::cartesian_coordinate<float, 3> const location) const noexcept;

    [[nodiscard]]
    maths::cartesian_coordinate<float, 3, maths::layout::column_major> get_focal_point() const noexcept;

    [[nodiscard]]
    maths::cartesian_coordinate<float, 3, maths::layout::column_major> get_lens_normal() const noexcept;

    [[nodiscard]]
    maths::cartesian_coordinate<float, 3, maths::layout::column_major> get_lens_origin() const noexcept;

    [[nodiscard]]
    maths::cartesian_coordinate<float, 3, maths::layout::column_major> get_lens_x_basis() const noexcept;

    [[nodiscard]]
    maths::cartesian_coordinate<float, 3, maths::layout::column_major> get_lens_y_basis() const noexcept;

    [[nodiscard]]
    maths::cartesian_coordinate<float, 2, maths::layout::column_major> get_screen_size() const noexcept;

    [[nodiscard]]
    float get_focal_length() const noexcept;

    [[nodiscard]]
    float get_render_distance() const noexcept;

  private:
    maths::cartesian_coordinate<float, 3> focal_point;
    maths::cartesian_coordinate<float, 3> lens_normal;
    float                                 focal_length;
    float                                 roll;
    float                                 horizontal_field_of_view;
    float                                 virtual_field_of_view;
    float                                 render_distance;
};
}