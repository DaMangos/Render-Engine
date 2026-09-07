#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(RowViewDivisionAssignment, RowMajorRowViewOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 8, x[1][1] = 10, x[1][2] = 12;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    auto result = (x.row(1) /= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 8);
  static_assert(x[2][2] == 9);
}

TEST(RowViewDivisionAssignment, ColumnMajorRowViewOverScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 8, x[1][1] = 10, x[1][2] = 12;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    auto result = (x.row(1) /= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 8);
  static_assert(x[2][2] == 9);
}
}
