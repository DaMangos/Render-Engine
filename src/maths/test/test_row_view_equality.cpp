#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(RowViewEquality, RowMajorRowViewCompareRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  static_assert(x.row(0) == y.row(0));
  static_assert(x.row(1) == y.row(1));
  static_assert(x.row(0) != y.row(1));
  static_assert(x.row(1) != y.row(0));
}

TEST(RowViewEquality, ColumnMajorRowViewCompareRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  static_assert(x.row(0) == y.row(0));
  static_assert(x.row(1) == y.row(1));
  static_assert(x.row(0) != y.row(1));
  static_assert(x.row(1) != y.row(0));
}

TEST(RowViewEquality, RowMajorRowViewCompareColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  static_assert(x.row(0) == y.row(0));
  static_assert(x.row(1) == y.row(1));
  static_assert(x.row(0) != y.row(1));
  static_assert(x.row(1) != y.row(0));
}

TEST(RowViewEquality, ColumnMajorRowViewCompareColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  static_assert(x.row(0) == y.row(0));
  static_assert(x.row(1) == y.row(1));
  static_assert(x.row(0) != y.row(1));
  static_assert(x.row(1) != y.row(0));
}

TEST(RowViewEquality, RowMajorMatrixCompareRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  static_assert(x == y.row(0));
  static_assert(x != y.row(1));

  static_assert(y.row(0) == x);
  static_assert(y.row(1) != x);
}

TEST(RowViewEquality, ColumnMajorMatrixCompareRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  static_assert(x == y.row(0));
  static_assert(x != y.row(1));

  static_assert(y.row(0) == x);
  static_assert(y.row(1) != x);
}

TEST(RowViewEquality, RowMajorMatrixCompareColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  static_assert(x == y.row(0));
  static_assert(x != y.row(1));

  static_assert(y.row(0) == x);
  static_assert(y.row(1) != x);
}

TEST(RowViewEquality, ColumnMajorMatrixCompareColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  static_assert(x == y.row(0));
  static_assert(x != y.row(1));

  static_assert(y.row(0) == x);
  static_assert(y.row(1) != x);
}
}