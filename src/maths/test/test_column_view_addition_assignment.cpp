#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(ColumnViewAdditionAssignment, RowMajorColumnViewAddRowMajorColumnView)
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

  EXPECT_EQ(result[0], 5);
  EXPECT_EQ(result[1], 7);
  EXPECT_EQ(result[2], 9);

  EXPECT_EQ(x[0][0], 1);
  EXPECT_EQ(x[1][0], 4);
  EXPECT_EQ(x[2][0], 7);

  EXPECT_EQ(x[0][1], 5);
  EXPECT_EQ(x[1][1], 7);
  EXPECT_EQ(x[2][1], 9);

  EXPECT_EQ(x[0][2], 3);
  EXPECT_EQ(x[1][2], 6);
  EXPECT_EQ(x[2][2], 9);
}

TEST(ColumnViewAdditionAssignment, ColumnMajorColumnViewAddRowMajorColumnView)
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

  EXPECT_EQ(result[0], 5);
  EXPECT_EQ(result[1], 7);
  EXPECT_EQ(result[2], 9);

  EXPECT_EQ(x[0][0], 1);
  EXPECT_EQ(x[1][0], 4);
  EXPECT_EQ(x[2][0], 7);

  EXPECT_EQ(x[0][1], 5);
  EXPECT_EQ(x[1][1], 7);
  EXPECT_EQ(x[2][1], 9);

  EXPECT_EQ(x[0][2], 3);
  EXPECT_EQ(x[1][2], 6);
  EXPECT_EQ(x[2][2], 9);
}

TEST(ColumnViewAdditionAssignment, RowMajorColumnViewAddColumnMajorColumnView)
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

  EXPECT_EQ(result[0], 5);
  EXPECT_EQ(result[1], 7);
  EXPECT_EQ(result[2], 9);

  EXPECT_EQ(x[0][0], 1);
  EXPECT_EQ(x[1][0], 4);
  EXPECT_EQ(x[2][0], 7);

  EXPECT_EQ(x[0][1], 5);
  EXPECT_EQ(x[1][1], 7);
  EXPECT_EQ(x[2][1], 9);

  EXPECT_EQ(x[0][2], 3);
  EXPECT_EQ(x[1][2], 6);
  EXPECT_EQ(x[2][2], 9);
}

TEST(ColumnViewAdditionAssignment, ColumnMajorColumnViewAddColumnMajorColumnView)
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

  EXPECT_EQ(result[0], 5);
  EXPECT_EQ(result[1], 7);
  EXPECT_EQ(result[2], 9);

  EXPECT_EQ(x[0][0], 1);
  EXPECT_EQ(x[1][0], 4);
  EXPECT_EQ(x[2][0], 7);

  EXPECT_EQ(x[0][1], 5);
  EXPECT_EQ(x[1][1], 7);
  EXPECT_EQ(x[2][1], 9);

  EXPECT_EQ(x[0][2], 3);
  EXPECT_EQ(x[1][2], 6);
  EXPECT_EQ(x[2][2], 9);
}
}
