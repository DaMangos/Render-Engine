#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(ColumnViewAddition, RowMajorColumnViewAddRowMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x.column(0) + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(ColumnViewAddition, RowMajorColumnViewAddColumnMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x.column(0) + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(ColumnViewAddition, ColumnMajorColumnViewAddRowMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x.column(0) + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(ColumnViewAddition, ColumnMajorColumnViewAddColumnMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x.column(0) + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}
}