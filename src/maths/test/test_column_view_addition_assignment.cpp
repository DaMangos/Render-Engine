#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(ColumnViewAdditionAssignment, RowMajorColumnViewAddRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 3, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 2, y[1][2] = 6;
    y[2][0] = 7, y[2][1] = 1, y[2][2] = 9;

    auto result = (x.column(1) += y.column(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::column_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 4);
  static_assert(x[2][0] == 7);

  static_assert(x[0][1] == 5);
  static_assert(x[1][1] == 7);
  static_assert(x[2][1] == 9);

  static_assert(x[0][2] == 3);
  static_assert(x[1][2] == 6);
  static_assert(x[2][2] == 9);
}

TEST(ColumnViewAdditionAssignment, ColumnMajorColumnViewAddRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 3, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 2, y[1][2] = 6;
    y[2][0] = 7, y[2][1] = 1, y[2][2] = 9;

    auto result = (x.column(1) += y.column(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::column_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 4);
  static_assert(x[2][0] == 7);

  static_assert(x[0][1] == 5);
  static_assert(x[1][1] == 7);
  static_assert(x[2][1] == 9);

  static_assert(x[0][2] == 3);
  static_assert(x[1][2] == 6);
  static_assert(x[2][2] == 9);
}

TEST(ColumnViewAdditionAssignment, RowMajorColumnViewAddColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 3, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 2, y[1][2] = 6;
    y[2][0] = 7, y[2][1] = 1, y[2][2] = 9;

    auto result = (x.column(1) += y.column(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::column_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 4);
  static_assert(x[2][0] == 7);

  static_assert(x[0][1] == 5);
  static_assert(x[1][1] == 7);
  static_assert(x[2][1] == 9);

  static_assert(x[0][2] == 3);
  static_assert(x[1][2] == 6);
  static_assert(x[2][2] == 9);
}

TEST(ColumnViewAdditionAssignment, ColumnMajorColumnViewAddColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 3, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 2, y[1][2] = 6;
    y[2][0] = 7, y[2][1] = 1, y[2][2] = 9;

    auto result = (x.column(1) += y.column(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::column_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 4);
  static_assert(x[2][0] == 7);

  static_assert(x[0][1] == 5);
  static_assert(x[1][1] == 7);
  static_assert(x[2][1] == 9);

  static_assert(x[0][2] == 3);
  static_assert(x[1][2] == 6);
  static_assert(x[2][2] == 9);
}
}
