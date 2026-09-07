#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(ColumnViewDivision, RowMajorColumnViewOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    return x;
  }();

  constexpr auto result = x.column(2) / 3.0;

  static_assert(std::same_as<decltype(result), maths::matrix<double, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == 1.0);
  static_assert(result[1][0] == 2.0);
  static_assert(result[2][0] == 3.0);
}

TEST(ColumnViewDivision, ColumnMajorColumnViewOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    return x;
  }();

  constexpr auto result = x.column(2) / 3.0;

  static_assert(std::same_as<decltype(result), maths::matrix<double, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == 1.0);
  static_assert(result[1][0] == 2.0);
  static_assert(result[2][0] == 3.0);
}
}
