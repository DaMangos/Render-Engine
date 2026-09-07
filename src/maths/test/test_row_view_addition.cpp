#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(RowViewAddition, RowMajorRowViewAddRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  constexpr auto result = x.row(0) + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(y[0][0] == 1);
  static_assert(y[0][1] == 2);
  static_assert(y[0][2] == 3);

  static_assert(y[1][0] == 4);
  static_assert(y[1][1] == 5);
  static_assert(y[1][2] == 6);
}

TEST(RowViewAddition, ColumnMajorRowViewAddRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  constexpr auto result = x.row(0) + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(y[0][0] == 1);
  static_assert(y[0][1] == 2);
  static_assert(y[0][2] == 3);

  static_assert(y[1][0] == 4);
  static_assert(y[1][1] == 5);
  static_assert(y[1][2] == 6);
}

TEST(RowViewAddition, RowMajorRowViewAddColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  constexpr auto result = x.row(0) + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(y[0][0] == 1);
  static_assert(y[0][1] == 2);
  static_assert(y[0][2] == 3);

  static_assert(y[1][0] == 4);
  static_assert(y[1][1] == 5);
  static_assert(y[1][2] == 6);
}

TEST(RowViewAddition, ColumnMajorRowViewAddColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  constexpr auto result = x.row(0) + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(y[0][0] == 1);
  static_assert(y[0][1] == 2);
  static_assert(y[0][2] == 3);

  static_assert(y[1][0] == 4);
  static_assert(y[1][1] == 5);
  static_assert(y[1][2] == 6);
}
}