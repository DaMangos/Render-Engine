#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(ColumnViewAddition, RowMajorColumnViewAddRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x.column(0) + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(ColumnViewAddition, ColumnMajorColumnViewAddRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;
    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;
    return y;
  }();

  constexpr auto result = x.column(0) + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(ColumnViewAddition, RowMajorColumnViewAddColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x.column(0) + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(ColumnViewAddition, ColumnMajorColumnViewAddColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x.column(0) + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}
}