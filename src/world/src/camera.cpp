#include <world/camera.hpp>

void world::camera::set_pitch(float const pitch) const
{
}

void world::camera::set_yaw(float const yaw) const
{
}

void world::camera::set_roll(float const roll) const
{
}

void world::camera::set_pos(maths::cartesian_coordinate<float, 3> const pos) const noexcept
{
}

void world::camera::look_at(maths::cartesian_coordinate<float, 3> const location) const noexcept
{
}

maths::cartesian_coordinate<float, 3, maths::layout::column_major> world::camera::get_focal_point() const noexcept
{
  return focal_point;
}

maths::cartesian_coordinate<float, 3, maths::layout::column_major> world::camera::get_lens_normal() const noexcept
{
  return lens_normal;
}

maths::cartesian_coordinate<float, 3, maths::layout::column_major> world::camera::get_lens_origin() const noexcept
{
  return maths::cartesian_coordinate{focal_point - (focal_length * lens_normal)};
}

maths::cartesian_coordinate<float, 3, maths::layout::column_major> world::camera::get_lens_x_basis() const noexcept
{
}

maths::cartesian_coordinate<float, 3, maths::layout::column_major> world::camera::get_lens_y_basis() const noexcept
{
}

maths::cartesian_coordinate<float, 2, maths::layout::column_major> world::camera::get_screen_size() const noexcept
{
}

float world::camera::get_focal_length() const noexcept
{
}

float world::camera::get_render_distance() const noexcept
{
}