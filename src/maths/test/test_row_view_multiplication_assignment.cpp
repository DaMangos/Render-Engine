#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(RowViewMultiplicationAssignment, RowMajorRowViewTimesScalar)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
  x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

  auto result = (x.row(1) *= 2);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::row_major>::row_view>);

  EXPECT_EQ(result[0], 8);
  EXPECT_EQ(result[1], 10);
  EXPECT_EQ(result[2], 12);

  EXPECT_EQ(x[0][0], 1);
  EXPECT_EQ(x[0][1], 2);
  EXPECT_EQ(x[0][2], 3);

  EXPECT_EQ(x[1][0], 8);
  EXPECT_EQ(x[1][1], 10);
  EXPECT_EQ(x[1][2], 12);

  EXPECT_EQ(x[2][0], 7);
  EXPECT_EQ(x[2][1], 8);
  EXPECT_EQ(x[2][2], 9);
}

TEST(RowViewMultiplicationAssignment, ColumnMajorRowViewTimesScalar)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
  x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

  auto result = (x.row(1) *= 2);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 3, maths::layout::column_major>::row_view>);

  EXPECT_EQ(result[0], 8);
  EXPECT_EQ(result[1], 10);
  EXPECT_EQ(result[2], 12);

  EXPECT_EQ(x[0][0], 1);
  EXPECT_EQ(x[0][1], 2);
  EXPECT_EQ(x[0][2], 3);

  EXPECT_EQ(x[1][0], 8);
  EXPECT_EQ(x[1][1], 10);
  EXPECT_EQ(x[1][2], 12);

  EXPECT_EQ(x[2][0], 7);
  EXPECT_EQ(x[2][1], 8);
  EXPECT_EQ(x[2][2], 9);
}
}
