#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <forward_list>
#include <mdspan>
#include <vector>

namespace
{
TEST(ColumnViewEquality, RowMajorColumnViewCompareRowMajorColumnView)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
  x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
  x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  EXPECT_TRUE(x.column(0) == y.column(0));
  EXPECT_TRUE(x.column(1) == y.column(1));
  EXPECT_TRUE(x.column(0) != y.column(1));
  EXPECT_TRUE(x.column(1) != y.column(0));
}

TEST(ColumnViewEquality, ColumnMajorColumnViewCompareRowMajorColumnView)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
  x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
  x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  EXPECT_TRUE(x.column(0) == y.column(0));
  EXPECT_TRUE(x.column(1) == y.column(1));
  EXPECT_TRUE(x.column(0) != y.column(1));
  EXPECT_TRUE(x.column(1) != y.column(0));
}

TEST(ColumnViewEquality, RowMajorColumnViewCompareColumnMajorColumnView)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
  x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
  x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  EXPECT_TRUE(x.column(0) == y.column(0));
  EXPECT_TRUE(x.column(1) == y.column(1));
  EXPECT_TRUE(x.column(0) != y.column(1));
  EXPECT_TRUE(x.column(1) != y.column(0));
}

TEST(ColumnViewEquality, ColumnMajorColumnViewCompareColumnMajorColumnView)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
  x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
  x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  EXPECT_TRUE(x.column(0) == y.column(0));
  EXPECT_TRUE(x.column(1) == y.column(1));
  EXPECT_TRUE(x.column(0) != y.column(1));
  EXPECT_TRUE(x.column(1) != y.column(0));
}

TEST(ColumnViewEquality, RowMajorMatrixCompareRowMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::row_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  EXPECT_TRUE(x == y.column(0));
  EXPECT_TRUE(x != y.column(1));

  EXPECT_TRUE(y.column(0) == x);
  EXPECT_TRUE(y.column(1) != x);
}

TEST(ColumnViewEquality, ColumnMajorMatrixCompareRowMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::column_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  EXPECT_TRUE(x == y.column(0));
  EXPECT_TRUE(x != y.column(1));

  EXPECT_TRUE(y.column(0) == x);
  EXPECT_TRUE(y.column(1) != x);
}

TEST(ColumnViewEquality, RowMajorMatrixCompareColumnMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::row_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  EXPECT_TRUE(x == y.column(0));
  EXPECT_TRUE(x != y.column(1));

  EXPECT_TRUE(y.column(0) == x);
  EXPECT_TRUE(y.column(1) != x);
}

TEST(ColumnViewEquality, ColumnMajorMatrixCompareColumnMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::column_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  EXPECT_TRUE(x == y.column(0));
  EXPECT_TRUE(x != y.column(1));

  EXPECT_TRUE(y.column(0) == x);
  EXPECT_TRUE(y.column(1) != x);
}
}