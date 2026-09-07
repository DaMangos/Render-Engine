#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(ColumnViewEquality, RowMajorColumnViewCompareRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

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

  static_assert(x.column(0) == y.column(0));
  static_assert(x.column(1) == y.column(1));
  static_assert(x.column(0) != y.column(1));
  static_assert(x.column(1) != y.column(0));
}

TEST(ColumnViewEquality, ColumnMajorColumnViewCompareRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

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

  static_assert(x.column(0) == y.column(0));
  static_assert(x.column(1) == y.column(1));
  static_assert(x.column(0) != y.column(1));
  static_assert(x.column(1) != y.column(0));
}

TEST(ColumnViewEquality, RowMajorColumnViewCompareColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

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

  static_assert(x.column(0) == y.column(0));
  static_assert(x.column(1) == y.column(1));
  static_assert(x.column(0) != y.column(1));
  static_assert(x.column(1) != y.column(0));
}

TEST(ColumnViewEquality, ColumnMajorColumnViewCompareColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

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

  static_assert(x.column(0) == y.column(0));
  static_assert(x.column(1) == y.column(1));
  static_assert(x.column(0) != y.column(1));
  static_assert(x.column(1) != y.column(0));
}

TEST(ColumnViewEquality, RowMajorMatrixCompareRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 1, maths::layout::row_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

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

  static_assert(x == y.column(0));
  static_assert(x != y.column(1));

  static_assert(y.column(0) == x);
  static_assert(y.column(1) != x);
}

TEST(ColumnViewEquality, ColumnMajorMatrixCompareRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 1, maths::layout::column_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

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

  static_assert(x == y.column(0));
  static_assert(x != y.column(1));

  static_assert(y.column(0) == x);
  static_assert(y.column(1) != x);
}

TEST(ColumnViewEquality, RowMajorMatrixCompareColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 1, maths::layout::row_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

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

  static_assert(x == y.column(0));
  static_assert(x != y.column(1));

  static_assert(y.column(0) == x);
  static_assert(y.column(1) != x);
}

TEST(ColumnViewEquality, ColumnMajorMatrixCompareColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 1, maths::layout::column_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

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

  static_assert(x == y.column(0));
  static_assert(x != y.column(1));

  static_assert(y.column(0) == x);
  static_assert(y.column(1) != x);
}
}