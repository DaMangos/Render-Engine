#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(RowViewDivision, RowMajorRowViewOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

    return x;
  }();

  constexpr auto result = x.row(2) / 3.0;

  static_assert(std::same_as<decltype(result), maths::matrix<double, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == 1.0);
  static_assert(result[0][1] == 2.0);
  static_assert(result[0][2] == 3.0);
}

TEST(RowViewDivision, ColumnMajorRowViewOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

    return x;
  }();

  constexpr auto result = x.row(2) / 3.0;

  static_assert(std::same_as<decltype(result), maths::matrix<double, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == 1.0);
  static_assert(result[0][1] == 2.0);
  static_assert(result[0][2] == 3.0);
}
}
