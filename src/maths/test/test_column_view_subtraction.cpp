#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(ColumnViewSubtraction, RowMajorColumnViewMinusRowMajorColumnView)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x.column(0) - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 2);
  static_assert(x[2][0] == 3);

  static_assert(x[0][1] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[2][1] == 6);

  static_assert(y[0][0] == 2);
  static_assert(y[1][0] == 4);
  static_assert(y[2][0] == 6);

  static_assert(y[0][1] == 8);
  static_assert(y[1][1] == 10);
  static_assert(y[2][1] == 12);
}

TEST(ColumnViewSubtraction, ColumnMajorColumnViewMinusRowMajorColumnView)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x.column(0) - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 2);
  static_assert(x[2][0] == 3);

  static_assert(x[0][1] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[2][1] == 6);

  static_assert(y[0][0] == 2);
  static_assert(y[1][0] == 4);
  static_assert(y[2][0] == 6);

  static_assert(y[0][1] == 8);
  static_assert(y[1][1] == 10);
  static_assert(y[2][1] == 12);
}

TEST(ColumnViewSubtraction, RowMajorColumnViewMinusColumnMajorColumnView)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x.column(0) - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 2);
  static_assert(x[2][0] == 3);

  static_assert(x[0][1] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[2][1] == 6);

  static_assert(y[0][0] == 2);
  static_assert(y[1][0] == 4);
  static_assert(y[2][0] == 6);

  static_assert(y[0][1] == 8);
  static_assert(y[1][1] == 10);
  static_assert(y[2][1] == 12);
}

TEST(ColumnViewSubtraction, ColumnMajorColumnViewMinusColumnMajorColumnView)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x.column(0) - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);

  static_assert(x[0][0] == 1);
  static_assert(x[1][0] == 2);
  static_assert(x[2][0] == 3);

  static_assert(x[0][1] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[2][1] == 6);

  static_assert(y[0][0] == 2);
  static_assert(y[1][0] == 4);
  static_assert(y[2][0] == 6);

  static_assert(y[0][1] == 8);
  static_assert(y[1][1] == 10);
  static_assert(y[2][1] == 12);
}
}