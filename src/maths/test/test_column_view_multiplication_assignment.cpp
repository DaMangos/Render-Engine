#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(ColumnViewMultiplicationAssignment, RowMajorColumnViewTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

    auto result = (x.column(1) *= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::column_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 2);
  static_assert(x[2][0] == 3);

  static_assert(x[0][1] == 8);
  static_assert(x[1][1] == 10);
  static_assert(x[2][1] == 12);

  static_assert(x[0][2] == 7);
  static_assert(x[1][2] == 8);
  static_assert(x[2][2] == 9);
}

TEST(ColumnViewMultiplicationAssignment, ColumnMajorColumnViewTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

    auto result = (x.column(1) *= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::column_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 2);
  static_assert(x[2][0] == 3);

  static_assert(x[0][1] == 8);
  static_assert(x[1][1] == 10);
  static_assert(x[2][1] == 12);

  static_assert(x[0][2] == 7);
  static_assert(x[1][2] == 8);
  static_assert(x[2][2] == 9);
}
}
