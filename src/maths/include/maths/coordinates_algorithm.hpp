#pragma once

#include <maths/coordinates.hpp>

#include <numeric>
#include <type_traits>

namespace maths
{
template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, layout LhsLayout, layout RhsLayout>
[[nodiscard]]
constexpr std::common_type_t<LhsArithmetic, RhsArithmetic> dot(
  cartesian_coordinate<LhsArithmetic, ColumnSize, LhsLayout> const & lhs,
  cartesian_coordinate<RhsArithmetic, ColumnSize, RhsLayout> const & rhs) noexcept
{
  return std::inner_product(lhs.column(0).begin(),
                            lhs.column(0).end(),
                            rhs.column(0).begin(),
                            std::common_type_t<LhsArithmetic, RhsArithmetic>{0});
}

template <class LhsArithmetic, class RhsArithmetic, layout LhsLayout, layout RhsLayout>
[[nodiscard]]
constexpr cartesian_coordinate<std::common_type_t<LhsArithmetic, RhsArithmetic>, 3uz, LhsLayout> cross(
  cartesian_coordinate<LhsArithmetic, 3uz, LhsLayout> const & lhs,
  cartesian_coordinate<RhsArithmetic, 3uz, RhsLayout> const & rhs) noexcept
{
  return {lhs.y() * rhs.z() - lhs.z() * rhs.y(), lhs.z() * rhs.x() - lhs.x() * rhs.z(), lhs.x() * rhs.y() - lhs.y() * rhs.x()};
}
}