#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(MatrixDivision, RowMajorMatrixOverScalar)
{
  maths::matrix<int, 2, 2, maths::layout::row_major> x;

  x[0][0] = 2, x[0][1] = 4;
  x[1][0] = 8, x[1][1] = 16;

  auto result = x / 2.0;

  static_assert(std::same_as<decltype(result), maths::matrix<double, 2, 2, maths::layout::row_major>>);

  EXPECT_DOUBLE_EQ(result[0][0], 1.0);
  EXPECT_DOUBLE_EQ(result[0][1], 2.0);
  EXPECT_DOUBLE_EQ(result[1][0], 4.0);
  EXPECT_DOUBLE_EQ(result[1][1], 8.0);
}

TEST(MatrixDivision, ColumnMajorMatrixOverScalar)
{
  maths::matrix<int, 2, 2, maths::layout::column_major> x;

  x[0][0] = 2, x[0][1] = 4;
  x[1][0] = 8, x[1][1] = 16;

  auto result = x / 2.0;

  static_assert(std::same_as<decltype(result), maths::matrix<double, 2, 2, maths::layout::column_major>>);

  EXPECT_DOUBLE_EQ(result[0][0], 1.0);
  EXPECT_DOUBLE_EQ(result[0][1], 2.0);
  EXPECT_DOUBLE_EQ(result[1][0], 4.0);
  EXPECT_DOUBLE_EQ(result[1][1], 8.0);
}
}
