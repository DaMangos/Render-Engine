#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(ColumnViewDivisionAssignment, RowMajorColumnViewOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 8, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 10, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 12, x[2][2] = 9;

    auto result = (x.column(1) /= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::column_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 4);
  static_assert(x[2][0] == 7);

  static_assert(x[0][1] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[2][1] == 6);

  static_assert(x[0][2] == 3);
  static_assert(x[1][2] == 6);
  static_assert(x[2][2] == 9);
}

TEST(ColumnViewDivisionAssignment, ColumnMajorColumnViewOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 8, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 10, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 12, x[2][2] = 9;

    auto result = (x.column(1) /= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::column_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 4);
  static_assert(x[2][0] == 7);

  static_assert(x[0][1] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[2][1] == 6);

  static_assert(x[0][2] == 3);
  static_assert(x[1][2] == 6);
  static_assert(x[2][2] == 9);
}
}
