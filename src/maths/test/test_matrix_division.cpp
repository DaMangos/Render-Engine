#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(MatrixDivision, RowMajorMatrixOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 2, maths::layout::row_major> x;

    x[0][0] = 2, x[0][1] = 4;
    x[1][0] = 8, x[1][1] = 16;

    return x;
  }();

  constexpr auto result = x / 2.0;

  static_assert(std::same_as<decltype(result), maths::matrix<double, 2, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 1.0);
  static_assert(result[0][1] == 2.0);
  static_assert(result[1][0] == 4.0);
  static_assert(result[1][1] == 8.0);
}

TEST(MatrixDivision, ColumnMajorMatrixOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 2, maths::layout::column_major> x;

    x[0][0] = 2, x[0][1] = 4;
    x[1][0] = 8, x[1][1] = 16;

    return x;
  }();

  constexpr auto result = x / 2.0;

  static_assert(std::same_as<decltype(result), maths::matrix<double, 2, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 1.0);
  static_assert(result[0][1] == 2.0);
  static_assert(result[1][0] == 4.0);
  static_assert(result[1][1] == 8.0);
}
}
