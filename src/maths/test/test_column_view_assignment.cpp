#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(ColumnViewAssignment, RowMajorColumnViewToRowMajorColumnView)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 4, from[0][2] = 7;
  from[1][0] = 2, from[1][1] = 5, from[1][2] = 8;
  from[2][0] = 3, from[2][1] = 6, from[2][2] = 9;

  maths::matrix<int, 3, 2, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = from.column(0);

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}

TEST(ColumnViewAssignment, ColumnMajorColumnViewToRowMajorColumnView)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 4, from[0][2] = 7;
  from[1][0] = 2, from[1][1] = 5, from[1][2] = 8;
  from[2][0] = 3, from[2][1] = 6, from[2][2] = 9;

  maths::matrix<int, 3, 2, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = from.column(0);

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}

TEST(ColumnViewAssignment, RowMajorColumnViewToColumnMajorColumnView)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 4, from[0][2] = 7;
  from[1][0] = 2, from[1][1] = 5, from[1][2] = 8;
  from[2][0] = 3, from[2][1] = 6, from[2][2] = 9;

  maths::matrix<int, 3, 2, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = from.column(0);

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}

TEST(ColumnViewAssignment, ColumnMajorColumnViewToColumnMajorColumnView)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 4, from[0][2] = 7;
  from[1][0] = 2, from[1][1] = 5, from[1][2] = 8;
  from[2][0] = 3, from[2][1] = 6, from[2][2] = 9;

  maths::matrix<int, 3, 2, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = from.column(0);

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}

TEST(ColumnViewAssignment, RowMajorMatrixToRowMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::row_major> from;

  from[0][0] = 1;
  from[1][0] = 2;
  from[2][0] = 3;

  maths::matrix<int, 3, 2, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = from;

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}

TEST(ColumnViewAssignment, ColumnMajorMatrixToRowMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::column_major> from;

  from[0][0] = 1;
  from[1][0] = 2;
  from[2][0] = 3;

  maths::matrix<int, 3, 2, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = from;

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}

TEST(ColumnViewAssignment, RowMajorMatrixToColumnMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::row_major> from;

  from[0][0] = 1;
  from[1][0] = 2;
  from[2][0] = 3;

  maths::matrix<int, 3, 2, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = from;

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}

TEST(ColumnViewAssignment, ColumnMajorMatrixToColumnMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::column_major> from;

  from[0][0] = 1;
  from[1][0] = 2;
  from[2][0] = 3;

  maths::matrix<int, 3, 2, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = from;

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}

TEST(ColumnViewAssignment, InitializerListToRowMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = {1, 2, 3};

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}

TEST(ColumnViewAssignment, InitializerListToColumnMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 4;
  to[1][0] = 2, to[1][1] = 5;
  to[2][0] = 3, to[2][1] = 6;

  to.column(1) = {1, 2, 3};

  EXPECT_EQ(to[0][0], to[0][1]);
  EXPECT_EQ(to[1][0], to[1][1]);
  EXPECT_EQ(to[2][0], to[2][1]);
}
}