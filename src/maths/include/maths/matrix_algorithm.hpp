#pragma once

#include <maths/matrix.hpp>

#include <cmath>
#include <type_traits>

namespace maths
{
template <class FloatingPoint, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
requires(std::is_floating_point_v<FloatingPoint>)
constexpr void gaussian_elimination(matrix<FloatingPoint, ColumnSize, RowSize, Layout> & mat) noexcept
{
}

template <class Arithmetic, std::size_t Size, layout Layout>
constexpr void inverse(matrix<Arithmetic, Size, Size, Layout> & mat) noexcept
{
}

template <class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr void transpose(matrix<Arithmetic, ColumnSize, RowSize, Layout> & mat) noexcept
{
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, layout LhsLayout, layout RhsLayout>
[[nodiscard]]
constexpr std::common_type_t<LhsArithmetic, RhsArithmetic> dot(
  matrix<LhsArithmetic, ColumnSize, 1uz, LhsLayout> const & lhs,
  matrix<RhsArithmetic, ColumnSize, 1uz, RhsLayout> const & rhs) noexcept
{
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, layout LhsLayout, layout RhsLayout>
[[nodiscard]]
constexpr std::common_type_t<LhsArithmetic, RhsArithmetic> cross(
  matrix<LhsArithmetic, ColumnSize, 1uz, LhsLayout> const & lhs,
  matrix<RhsArithmetic, ColumnSize, 1uz, RhsLayout> const & rhs) noexcept
{
}

template <class Arithmetic, std::size_t ColumnSize, layout Layout>
[[nodiscard]]
constexpr Arithmetic determinant(matrix<Arithmetic, ColumnSize, ColumnSize, Layout> const & mat) noexcept
{
}

template <class Arithmetic, std::size_t ColumnSize, layout Layout>
[[nodiscard]]
constexpr Arithmetic length_squared(matrix<Arithmetic, ColumnSize, 1uz, Layout> const & mat) noexcept
{
  return dot(mat, mat);
}

template <class FloatingPoint, std::size_t ColumnSize, layout Layout>
requires(std::floating_point<FloatingPoint>)
[[nodiscard]]
constexpr FloatingPoint length(matrix<FloatingPoint, ColumnSize, 1uz, Layout> const & mat) noexcept
{
  return std::sqrt(length_squared(mat));
}

namespace polar_coordinates
{
template <class FloatingPoint, layout Layout>
requires(std::floating_point<FloatingPoint>)
[[nodiscard]]
constexpr FloatingPoint radius(matrix<FloatingPoint, 2uz, 1uz, Layout> const & mat) noexcept
{
}

template <class FloatingPoint, layout Layout>
requires(std::floating_point<FloatingPoint>)
[[nodiscard]]
constexpr FloatingPoint theta(matrix<FloatingPoint, 2uz, 1uz, Layout> const & mat) noexcept
{
}

namespace spherical_coordinates
{
template <class FloatingPoint, layout Layout>
requires(std::floating_point<FloatingPoint>)
[[nodiscard]]
constexpr FloatingPoint radius(matrix<FloatingPoint, 3uz, 1uz, Layout> const & mat) noexcept
{
}

template <class FloatingPoint, layout Layout>
requires(std::floating_point<FloatingPoint>)
[[nodiscard]]
constexpr FloatingPoint theta(matrix<FloatingPoint, 3uz, 1uz, Layout> const & mat) noexcept
{
}

template <class FloatingPoint, layout Layout>
requires(std::floating_point<FloatingPoint>)
[[nodiscard]]
constexpr FloatingPoint phi(matrix<FloatingPoint, 3uz, 1uz, Layout> const & mat) noexcept
{
}
}
}
}