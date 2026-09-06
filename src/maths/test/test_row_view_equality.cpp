#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(RowViewEquality, RowMajorRowViewCompareRowMajorRowView)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
  x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  EXPECT_TRUE(x.row(0) == y.row(0));
  EXPECT_TRUE(x.row(1) == y.row(1));
  EXPECT_TRUE(x.row(0) != y.row(1));
  EXPECT_TRUE(x.row(1) != y.row(0));
}

TEST(RowViewEquality, ColumnMajorRowViewCompareRowMajorRowView)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
  x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  EXPECT_TRUE(x.row(0) == y.row(0));
  EXPECT_TRUE(x.row(1) == y.row(1));
  EXPECT_TRUE(x.row(0) != y.row(1));
  EXPECT_TRUE(x.row(1) != y.row(0));
}

TEST(RowViewEquality, RowMajorRowViewCompareColumnMajorRowView)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
  x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  EXPECT_TRUE(x.row(0) == y.row(0));
  EXPECT_TRUE(x.row(1) == y.row(1));
  EXPECT_TRUE(x.row(0) != y.row(1));
  EXPECT_TRUE(x.row(1) != y.row(0));
}

TEST(RowViewEquality, ColumnMajorRowViewCompareColumnMajorRowView)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;
  x[2][0] = 7, x[2][1] = 8, x[2][2] = 9;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  EXPECT_TRUE(x.row(0) == y.row(0));
  EXPECT_TRUE(x.row(1) == y.row(1));
  EXPECT_TRUE(x.row(0) != y.row(1));
  EXPECT_TRUE(x.row(1) != y.row(0));
}

TEST(RowViewEquality, RowMajorMatrixCompareRowMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  EXPECT_TRUE(x == y.row(0));
  EXPECT_TRUE(x != y.row(1));

  EXPECT_TRUE(y.row(0) == x);
  EXPECT_TRUE(y.row(1) != x);
}

TEST(RowViewEquality, ColumnMajorMatrixCompareRowMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  EXPECT_TRUE(x == y.row(0));
  EXPECT_TRUE(x != y.row(1));

  EXPECT_TRUE(y.row(0) == x);
  EXPECT_TRUE(y.row(1) != x);
}

TEST(RowViewEquality, RowMajorMatrixCompareColumnMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  EXPECT_TRUE(x == y.row(0));
  EXPECT_TRUE(x != y.row(1));

  EXPECT_TRUE(y.row(0) == x);
  EXPECT_TRUE(y.row(1) != x);
}

TEST(RowViewEquality, ColumnMajorMatrixCompareColumnMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  EXPECT_TRUE(x == y.row(0));
  EXPECT_TRUE(x != y.row(1));

  EXPECT_TRUE(y.row(0) == x);
  EXPECT_TRUE(y.row(1) != x);
}
}