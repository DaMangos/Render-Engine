#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(RowViewAssignment, RowMajorRowViewToRowMajorRowView)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 2, 3, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = from.row(0);

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}

TEST(RowViewAssignment, ColumnMajorRowViewToRowMajorRowView)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 2, 3, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = from.row(0);

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}

TEST(RowViewAssignment, RowMajorRowViewToColumnMajorRowView)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 2, 3, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = from.row(0);

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}

TEST(RowViewAssignment, ColumnMajorRowViewToColumnMajorRowView)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 2, 3, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = from.row(0);

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}

TEST(RowViewAssignment, RowMajorMatrixToRowMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;

  maths::matrix<int, 2, 3, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = from;

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}

TEST(RowViewAssignment, ColumnMajorMatrixToRowMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;

  maths::matrix<int, 2, 3, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = from;

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}

TEST(RowViewAssignment, RowMajorMatrixToColumnMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;

  maths::matrix<int, 2, 3, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = from;

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}

TEST(RowViewAssignment, ColumnMajorMatrixToColumnMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;

  maths::matrix<int, 2, 3, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = from;

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}

TEST(RowViewAssignment, InitializerListToRowMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = {1, 2, 3};

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}

TEST(RowViewAssignment, InitializerListToColumnMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> to;

  to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
  to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

  to.row(1) = {1, 2, 3};

  EXPECT_EQ(to[0][0], to[1][0]);
  EXPECT_EQ(to[0][1], to[1][1]);
  EXPECT_EQ(to[0][2], to[1][2]);
}
}