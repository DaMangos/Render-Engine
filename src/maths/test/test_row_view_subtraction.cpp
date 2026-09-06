#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(RowViewSubtraction, RowMajorRowViewMinusRowMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
  y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

  auto result = x.row(0) - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(RowViewSubtraction, RowMajorRowViewMinusColumnMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
  y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

  auto result = x.row(0) - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(RowViewSubtraction, ColumnMajorRowViewMinusRowMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
  y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

  auto result = x.row(0) - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(RowViewSubtraction, ColumnMajorRowViewMinusColumnMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
  y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

  auto result = x.row(0) - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}
}