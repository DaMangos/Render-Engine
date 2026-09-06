#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(ColumnViewMultiplicationAssignment, RowMajorColumnViewTimesScalar)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
  x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
  x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

  auto result = (x.column(1) *= 2);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::column_view>);

  EXPECT_EQ(result[0], 8);
  EXPECT_EQ(result[1], 10);
  EXPECT_EQ(result[2], 12);

  EXPECT_EQ(x[0][0], 1);
  EXPECT_EQ(x[1][0], 2);
  EXPECT_EQ(x[2][0], 3);

  EXPECT_EQ(x[0][1], 8);
  EXPECT_EQ(x[1][1], 10);
  EXPECT_EQ(x[2][1], 12);

  EXPECT_EQ(x[0][2], 7);
  EXPECT_EQ(x[1][2], 8);
  EXPECT_EQ(x[2][2], 9);
}

TEST(ColumnViewMultiplicationAssignment, ColumnMajorColumnViewTimesScalar)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
  x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
  x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

  auto result = (x.column(1) *= 2);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::column_view>);

  EXPECT_EQ(result[0], 8);
  EXPECT_EQ(result[1], 10);
  EXPECT_EQ(result[2], 12);

  EXPECT_EQ(x[0][0], 1);
  EXPECT_EQ(x[1][0], 2);
  EXPECT_EQ(x[2][0], 3);

  EXPECT_EQ(x[0][1], 8);
  EXPECT_EQ(x[1][1], 10);
  EXPECT_EQ(x[2][1], 12);

  EXPECT_EQ(x[0][2], 7);
  EXPECT_EQ(x[1][2], 8);
  EXPECT_EQ(x[2][2], 9);
}
}
