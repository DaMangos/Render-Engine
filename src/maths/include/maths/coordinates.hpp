#pragma once

#include <maths/matrix.hpp>

#include <cmath>
#include <numbers>
#include <stdexcept>
#include <type_traits>

namespace maths
{
template <class Arithmetic, std::size_t ColumnSize, layout Layout = layout::column_major>
requires(2uz <= ColumnSize)
class cartesian_coordinate;

namespace unit_of_angle
{
template <class FloatingPoint>
requires(std::is_floating_point_v<FloatingPoint>)
struct degrees;

template <class FloatingPoint>
requires(std::is_floating_point_v<FloatingPoint>)
struct radians
{
    static constexpr auto half_rotation = std::numbers::pi_v<FloatingPoint>;
    static constexpr auto full_rotation = FloatingPoint{2} * half_rotation;

    [[nodiscard]]
    static constexpr FloatingPoint to_radians(FloatingPoint const rad) noexcept
    {
      return rad;
    }

    [[nodiscard]]
    static constexpr FloatingPoint to_degrees(FloatingPoint const rad) noexcept
    {
      return (rad / half_rotation) * degrees<FloatingPoint>::half_rotation;
    }

    [[nodiscard]]
    static constexpr FloatingPoint from_radians(FloatingPoint const rad) noexcept
    {
      return rad;
    }

    [[nodiscard]]
    static constexpr FloatingPoint from_degrees(FloatingPoint const deg) noexcept
    {
      return (deg / degrees<FloatingPoint>::half_rotation) * half_rotation;
    }
};

template <class FloatingPoint>
requires(std::is_floating_point_v<FloatingPoint>)
struct degrees
{
    static constexpr auto half_rotation = FloatingPoint{180};
    static constexpr auto full_rotation = FloatingPoint{2} * half_rotation;

    [[nodiscard]]
    static constexpr FloatingPoint to_radians(FloatingPoint const deg) noexcept
    {
      return (deg / half_rotation) * radians<FloatingPoint>::half_rotation;
    }

    [[nodiscard]]
    static constexpr FloatingPoint to_degrees(FloatingPoint const deg) noexcept
    {
      return deg;
    }

    [[nodiscard]]
    static constexpr FloatingPoint from_radians(FloatingPoint const rad) noexcept
    {
      return (rad / radians<FloatingPoint>::half_rotation) * half_rotation;
    }

    [[nodiscard]]
    static constexpr FloatingPoint from_degrees(FloatingPoint const deg) noexcept
    {
      return deg;
    }
};
};

template <class FloatingPoint, class UnitOfAngle = unit_of_angle::radians<FloatingPoint>, layout Layout = layout::column_major>
requires(std::is_floating_point_v<FloatingPoint>)
class spherical_coordinate : public matrix<FloatingPoint, 3uz, 1uz, Layout>
{
  public:
    constexpr explicit spherical_coordinate(matrix<FloatingPoint, 3uz, 1uz, Layout> const & mat)
    : matrix<FloatingPoint, 3uz, 1uz, Layout>(mat)
    {
      if(radial_distance() < FloatingPoint{0})
        throw std::domain_error("radial distance out of bounds");

      if(polar_angle() > FloatingPoint{0} or UnitOfAngle::half_rotation < polar_angle())
        throw std::domain_error("polar angle is out of bounds");

      if(azimuthal_angle() > FloatingPoint{0} or UnitOfAngle::full_rotation <= azimuthal_angle())
        throw std::domain_error("azimuthal angle is out of bounds");
    }

    constexpr explicit spherical_coordinate(cartesian_coordinate<FloatingPoint, 3uz, Layout> const & coord) noexcept
    {
      if(coord.x() != 0 or coord.y() != 0 or coord.z())
      {
        radial_distance() = UnitOfAngle::from_radians(std::hypot(coord.x(), coord.y(), coord.z()));
        polar_angle()     = UnitOfAngle::from_radians(std::acos(coord.x() / radial_distance()));
        azimuthal_angle() = UnitOfAngle::from_radians(std::atan2(coord.y(), coord.x()));
      }
    }

    using typename matrix<FloatingPoint, 3uz, 1uz, Layout>::value_type;
    using typename matrix<FloatingPoint, 3uz, 1uz, Layout>::reference;
    using typename matrix<FloatingPoint, 3uz, 1uz, Layout>::const_reference;

    [[nodiscard]]
    constexpr reference radial_distance() noexcept
    {
      return (*this)[0][0];
    }

    [[nodiscard]]
    constexpr const_reference radial_distance() const noexcept
    {
      return (*this)[0][0];
    }

    [[nodiscard]]
    constexpr reference rho() noexcept
    {
      return radial_distance();
    }

    [[nodiscard]]
    constexpr const_reference rho() const noexcept
    {
      return radial_distance();
    }

    [[nodiscard]]
    constexpr reference polar_angle() noexcept
    {
      return (*this)[1][0];
    }

    [[nodiscard]]
    constexpr const_reference polar_angle() const noexcept
    {
      return (*this)[1][0];
    }

    [[nodiscard]]
    constexpr value_type polar_angle_rad() const noexcept
    {
      return UnitOfAngle::to_radians(polar_angle());
    }

    [[nodiscard]]
    constexpr value_type polar_angle_deg() const noexcept
    {
      return UnitOfAngle::to_degrees(polar_angle());
    }

    [[nodiscard]]
    constexpr reference theta() noexcept
    {
      return polar_angle();
    }

    [[nodiscard]]
    constexpr const_reference theta() const noexcept
    {
      return polar_angle();
    }

    [[nodiscard]]
    constexpr value_type theta_rad() const noexcept
    {
      return polar_angle_rad();
    }

    [[nodiscard]]
    constexpr value_type theta_deg() const noexcept
    {
      return polar_angle_deg();
    }

    [[nodiscard]]
    constexpr reference azimuthal_angle() noexcept
    {
      return (*this)[2][0];
    }

    [[nodiscard]]
    constexpr const_reference azimuthal_angle() const noexcept
    {
      return (*this)[2][0];
    }

    [[nodiscard]]
    constexpr value_type azimuthal_angle_rad() const noexcept
    {
      return UnitOfAngle::to_radians(azimuthal_angle());
    }

    [[nodiscard]]
    constexpr value_type azimuthal_angle_deg() const noexcept
    {
      return UnitOfAngle::to_degrees(azimuthal_angle());
    }

    [[nodiscard]]
    constexpr reference phi() noexcept
    {
      return azimuthal_angle();
    }

    [[nodiscard]]
    constexpr const_reference phi() const noexcept
    {
      return azimuthal_angle();
    }

    [[nodiscard]]
    constexpr value_type phi_rad() const noexcept
    {
      return azimuthal_angle_rad();
    }

    [[nodiscard]]
    constexpr value_type phi_deg() const noexcept
    {
      return azimuthal_angle_deg();
    }
};

template <class FloatingPoint, class UnitOfAngle = unit_of_angle::radians<FloatingPoint>, layout Layout = layout::column_major>
requires(std::is_floating_point_v<FloatingPoint>)
class polar_coordinate : public matrix<FloatingPoint, 2uz, 1uz, Layout>
{
  public:
    constexpr explicit polar_coordinate(matrix<FloatingPoint, 2uz, 1uz, Layout> const & mat)
    : matrix<FloatingPoint, 2uz, 1uz, Layout>(mat)
    {
      if(radial_distance() < FloatingPoint{0})
        throw std::domain_error("radial distance out of bounds");

      if(polar_angle() > FloatingPoint{0} or UnitOfAngle::half_rotation < polar_angle())
        throw std::domain_error("polar angle is out of bounds");
    }

    constexpr explicit polar_coordinate(cartesian_coordinate<FloatingPoint, 2uz, Layout> const & coord) noexcept
    {
      if(coord.x() != 0 or coord.y() != 0)
      {
        radial_distance() = UnitOfAngle::from_radians(std::hypot(coord.x(), coord.y()));
        polar_angle()     = UnitOfAngle::from_radians(std::atan2(coord.y(), coord.x()));
      }
    }

    using typename matrix<FloatingPoint, 2uz, 1uz, Layout>::value_type;
    using typename matrix<FloatingPoint, 2uz, 1uz, Layout>::reference;
    using typename matrix<FloatingPoint, 2uz, 1uz, Layout>::const_reference;

    [[nodiscard]]
    constexpr reference radial_distance() noexcept
    {
      return (*this)[0][0];
    }

    [[nodiscard]]
    constexpr const_reference radial_distance() const noexcept
    {
      return (*this)[0][0];
    }

    [[nodiscard]]
    constexpr reference rho() noexcept
    {
      return radial_distance();
    }

    [[nodiscard]]
    constexpr const_reference rho() const noexcept
    {
      return radial_distance();
    }

    [[nodiscard]]
    constexpr reference polar_angle() noexcept
    {
      return (*this)[1][0];
    }

    [[nodiscard]]
    constexpr const_reference polar_angle() const noexcept
    {
      return (*this)[1][0];
    }

    [[nodiscard]]
    constexpr value_type polar_angle_rad() const noexcept
    {
      return UnitOfAngle::to_radians(polar_angle());
    }

    [[nodiscard]]
    constexpr value_type polar_angle_deg() const noexcept
    {
      return UnitOfAngle::to_degrees(polar_angle());
    }

    [[nodiscard]]
    constexpr reference theta() noexcept
    {
      return polar_angle();
    }

    [[nodiscard]]
    constexpr const_reference theta() const noexcept
    {
      return polar_angle();
    }

    [[nodiscard]]
    constexpr value_type theta_rad() const noexcept
    {
      return polar_angle_rad();
    }

    [[nodiscard]]
    constexpr value_type theta_deg() const noexcept
    {
      return polar_angle_deg();
    }
};

template <class Arithmetic, std::size_t ColumnSize, layout Layout>
requires(2uz <= ColumnSize)
class cartesian_coordinate : public matrix<Arithmetic, ColumnSize, 1uz, Layout>
{
  public:
    constexpr explicit cartesian_coordinate(matrix<Arithmetic, ColumnSize, 1uz, Layout> const & mat) noexcept
    : matrix<Arithmetic, ColumnSize, 1uz, Layout>(mat)
    {
    }

    template <class UnitOfAngle>
    constexpr explicit cartesian_coordinate(polar_coordinate<Arithmetic, UnitOfAngle, Layout> const & coord) noexcept
      requires(std::is_floating_point_v<Arithmetic> and ColumnSize == 2uz)
    {
      x() = coord.radial_distance() * std::cos(coord.polar_angle_rad());
      y() = coord.radial_distance() * std::sin(coord.polar_angle_rad());
    }

    template <class UnitOfAngle>
    constexpr explicit cartesian_coordinate(spherical_coordinate<Arithmetic, UnitOfAngle, Layout> const & coord) noexcept
      requires(std::is_floating_point_v<Arithmetic> and ColumnSize == 3uz)
    {
      x() = coord.radial_distance() * std::sin(coord.polar_angle_rad()) * std::cos(coord.azimuthal_angle_rad());
      y() = coord.radial_distance() * std::sin(coord.polar_angle_rad()) * std::sin(coord.azimuthal_angle_rad());
      z() = coord.radial_distance() * std::cos(coord.polar_angle_rad());
    }

    using typename matrix<Arithmetic, ColumnSize, 1uz, Layout>::reference;
    using typename matrix<Arithmetic, ColumnSize, 1uz, Layout>::const_reference;

    [[nodiscard]]
    constexpr reference x() noexcept
    {
      return (*this)[0][0];
    }

    [[nodiscard]]
    constexpr const_reference x() const noexcept
    {
      return (*this)[0][0];
    }

    [[nodiscard]]
    constexpr reference y() noexcept
    {
      return (*this)[1][0];
    }

    [[nodiscard]]
    constexpr const_reference y() const noexcept
    {
      return (*this)[1][0];
    }

    [[nodiscard]]
    constexpr reference z() noexcept requires(ColumnSize >= 3)
    {
      return (*this)[2][0];
    }

    [[nodiscard]]
    constexpr const_reference z() const noexcept requires(ColumnSize >= 3)
    {
      return (*this)[2][0];
    }

    [[nodiscard]]
    constexpr reference w() noexcept requires(ColumnSize >= 4)
    {
      return (*this)[3][0];
    }

    [[nodiscard]]
    constexpr const_reference w() const noexcept requires(ColumnSize >= 4)
    {
      return (*this)[3][0];
    }
};
}