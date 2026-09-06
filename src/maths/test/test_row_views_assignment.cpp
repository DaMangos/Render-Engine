#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(RowViewsAssignment, RowMajorRowViewsToRowMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = from.rows();

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[0][2], from[0][2]);

  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[1][2], from[1][2]);

  EXPECT_EQ(to[2][0], from[2][0]);
  EXPECT_EQ(to[2][1], from[2][1]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(RowViewsAssignment, ColumnMajorRowViewsToRowMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = from.rows();

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[0][2], from[0][2]);

  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[1][2], from[1][2]);

  EXPECT_EQ(to[2][0], from[2][0]);
  EXPECT_EQ(to[2][1], from[2][1]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(RowViewsAssignment, RowMajorRowViewsToColumnMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = from.rows();

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[0][2], from[0][2]);

  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[1][2], from[1][2]);

  EXPECT_EQ(to[2][0], from[2][0]);
  EXPECT_EQ(to[2][1], from[2][1]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(RowViewsAssignment, ColumnMajorRowViewsToColumnMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = from.rows();

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[0][2], from[0][2]);

  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[1][2], from[1][2]);

  EXPECT_EQ(to[2][0], from[2][0]);
  EXPECT_EQ(to[2][1], from[2][1]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(RowViewsAssignment, RowMajorMatrixToRowMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = from;

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[0][2], from[0][2]);

  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[1][2], from[1][2]);

  EXPECT_EQ(to[2][0], from[2][0]);
  EXPECT_EQ(to[2][1], from[2][1]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(RowViewsAssignment, ColumnMajorMatrixToRowMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = from;

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[0][2], from[0][2]);

  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[1][2], from[1][2]);

  EXPECT_EQ(to[2][0], from[2][0]);
  EXPECT_EQ(to[2][1], from[2][1]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(RowViewsAssignment, RowMajorMatrixToColumnMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = from;

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[0][2], from[0][2]);

  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[1][2], from[1][2]);

  EXPECT_EQ(to[2][0], from[2][0]);
  EXPECT_EQ(to[2][1], from[2][1]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(RowViewsAssignment, ColumnMajorMatrixToColumnMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = from;

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[0][2], from[0][2]);

  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[1][2], from[1][2]);

  EXPECT_EQ(to[2][0], from[2][0]);
  EXPECT_EQ(to[2][1], from[2][1]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(RowViewsAssignment, InitializerListToRowMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
  };

  EXPECT_EQ(to[0][0], 1);
  EXPECT_EQ(to[0][1], 2);
  EXPECT_EQ(to[0][2], 3);

  EXPECT_EQ(to[1][0], 4);
  EXPECT_EQ(to[1][1], 5);
  EXPECT_EQ(to[1][2], 6);

  EXPECT_EQ(to[2][0], 7);
  EXPECT_EQ(to[2][1], 8);
  EXPECT_EQ(to[2][2], 9);
}

TEST(RowViewsAssignment, InitializerListToColumnMajorRowViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.rows() = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
  };

  EXPECT_EQ(to[0][0], 1);
  EXPECT_EQ(to[0][1], 2);
  EXPECT_EQ(to[0][2], 3);

  EXPECT_EQ(to[1][0], 4);
  EXPECT_EQ(to[1][1], 5);
  EXPECT_EQ(to[1][2], 6);

  EXPECT_EQ(to[2][0], 7);
  EXPECT_EQ(to[2][1], 8);
  EXPECT_EQ(to[2][2], 9);
}
}