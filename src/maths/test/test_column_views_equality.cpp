#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(ColumnViewsEquality, RowMajorMatrixCompareRowMajorMatrix)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
  x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
  x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

  maths::matrix<long, 3, 3, maths::layout::row_major> y;

  EXPECT_TRUE(x.columns() == x.columns());
  EXPECT_TRUE(x.columns() != y.columns());
  EXPECT_TRUE(y.columns() != x.columns());
  EXPECT_TRUE(y.columns() == y.columns());
}

TEST(ColumnViewsEquality, ColumnMajorMatrixCompareRowMajorMatrix)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
  x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
  x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

  maths::matrix<long, 3, 3, maths::layout::row_major> y;

  EXPECT_TRUE(x.columns() == x.columns());
  EXPECT_TRUE(x.columns() != y.columns());
  EXPECT_TRUE(y.columns() != x.columns());
  EXPECT_TRUE(y.columns() == y.columns());
}
}