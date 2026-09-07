#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(RowViewAdditionAssignment, RowMajorRowViewAddRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    auto result = (x.row(1) += y.row(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 8);
  static_assert(x[1][1] == 10);
  static_assert(x[1][2] == 12);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 8);
  static_assert(x[2][2] == 9);
}

TEST(RowViewAdditionAssignment, ColumnMajorRowViewAddRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    auto result = (x.row(1) += y.row(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 8);
  static_assert(x[1][1] == 10);
  static_assert(x[1][2] == 12);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 8);
  static_assert(x[2][2] == 9);
}

TEST(RowViewAdditionAssignment, RowMajorRowViewAddColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    auto result = (x.row(1) += y.row(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 8);
  static_assert(x[1][1] == 10);
  static_assert(x[1][2] == 12);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 8);
  static_assert(x[2][2] == 9);
}

TEST(RowViewAdditionAssignment, ColumnMajorRowViewAddColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
    x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    auto result = (x.row(1) += y.row(1));

    static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::row_view>);

    return x;
  }();

  static_assert(x[0][0] == 1);
  static_assert(x[0][1] == 2);
  static_assert(x[0][2] == 3);

  static_assert(x[1][0] == 8);
  static_assert(x[1][1] == 10);
  static_assert(x[1][2] == 12);

  static_assert(x[2][0] == 7);
  static_assert(x[2][1] == 8);
  static_assert(x[2][2] == 9);
}
}
