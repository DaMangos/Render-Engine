#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(RowViewSubtraction, RowMajorRowViewMinusRowMajorRowView)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
    y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

    return y;
  }();

  constexpr auto result = x.row(0) - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(RowViewSubtraction, ColumnMajorRowViewMinusRowMajorRowView)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
    y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

    return y;
  }();

  constexpr auto result = x.row(0) - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(RowViewSubtraction, RowMajorRowViewMinusColumnMajorRowView)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
    y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

    return y;
  }();

  constexpr auto result = x.row(0) - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(RowViewSubtraction, ColumnMajorRowViewMinusColumnMajorRowView)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
    y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

    return y;
  }();

  constexpr auto result = x.row(0) - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}
}