#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(RowViewSubtractionAssignment, RowMajorRowViewMinusRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 5, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 7, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 9, x[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 1, y[0][2] = 3;
    y[1][0] = 0, y[1][1] = 2, y[1][2] = 0;

    auto result = (x.row(1) -= y.row(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 5);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 9);
  static_assert(x[2][2] == 9);
}

TEST(RowViewSubtractionAssignment, ColumnMajorRowViewMinusRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 5, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 7, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 9, x[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 1, y[0][2] = 3;
    y[1][0] = 0, y[1][1] = 2, y[1][2] = 0;

    auto result = (x.row(1) -= y.row(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 5);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 9);
  static_assert(x[2][2] == 9);
}

TEST(RowViewSubtractionAssignment, RowMajorRowViewMinusColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 5, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 7, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 9, x[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 1, y[0][2] = 3;
    y[1][0] = 0, y[1][1] = 2, y[1][2] = 0;

    auto result = (x.row(1) -= y.row(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 5);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 9);
  static_assert(x[2][2] == 9);
}

TEST(RowViewSubtractionAssignment, ColumnMajorRowViewMinusColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 5, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 7, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 9, x[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 1, y[0][2] = 3;
    y[1][0] = 0, y[1][1] = 2, y[1][2] = 0;

    auto result = (x.row(1) -= y.row(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 5);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 4);
  static_assert(x[1][1] == 5);
  static_assert(x[1][2] == 6);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 9);
  static_assert(x[2][2] == 9);
}
}
