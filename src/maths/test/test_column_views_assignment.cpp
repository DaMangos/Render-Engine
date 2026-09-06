#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(ColumnViewsAssignment, RowMajorColumnViewsToRowMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = from.columns();

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[2][0], from[2][0]);

  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[2][1], from[2][1]);

  EXPECT_EQ(to[0][2], from[0][2]);
  EXPECT_EQ(to[1][2], from[1][2]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(ColumnViewsAssignment, ColumnMajorColumnViewsToRowMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = from.columns();

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[2][0], from[2][0]);

  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[2][1], from[2][1]);

  EXPECT_EQ(to[0][2], from[0][2]);
  EXPECT_EQ(to[1][2], from[1][2]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(ColumnViewsAssignment, RowMajorColumnViewsToColumnMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = from.columns();

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[2][0], from[2][0]);

  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[2][1], from[2][1]);

  EXPECT_EQ(to[0][2], from[0][2]);
  EXPECT_EQ(to[1][2], from[1][2]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(ColumnViewsAssignment, ColumnMajorColumnViewsToColumnMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = from.columns();

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[2][0], from[2][0]);

  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[2][1], from[2][1]);

  EXPECT_EQ(to[0][2], from[0][2]);
  EXPECT_EQ(to[1][2], from[1][2]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(ColumnViewsAssignment, RowMajorMatrixToRowMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = from;

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[2][0], from[2][0]);

  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[2][1], from[2][1]);

  EXPECT_EQ(to[0][2], from[0][2]);
  EXPECT_EQ(to[1][2], from[1][2]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(ColumnViewsAssignment, ColumnMajorMatrixToRowMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = from;

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[2][0], from[2][0]);

  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[2][1], from[2][1]);

  EXPECT_EQ(to[0][2], from[0][2]);
  EXPECT_EQ(to[1][2], from[1][2]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(ColumnViewsAssignment, RowMajorMatrixToColumnMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = from;

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[2][0], from[2][0]);

  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[2][1], from[2][1]);

  EXPECT_EQ(to[0][2], from[0][2]);
  EXPECT_EQ(to[1][2], from[1][2]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(ColumnViewsAssignment, ColumnMajorMatrixToColumnMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> from;

  from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
  from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
  from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = from;

  EXPECT_EQ(to[0][0], from[0][0]);
  EXPECT_EQ(to[1][0], from[1][0]);
  EXPECT_EQ(to[2][0], from[2][0]);

  EXPECT_EQ(to[0][1], from[0][1]);
  EXPECT_EQ(to[1][1], from[1][1]);
  EXPECT_EQ(to[2][1], from[2][1]);

  EXPECT_EQ(to[0][2], from[0][2]);
  EXPECT_EQ(to[1][2], from[1][2]);
  EXPECT_EQ(to[2][2], from[2][2]);
}

TEST(ColumnViewsAssignment, InitializerListToRowMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::row_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
  };

  EXPECT_EQ(to[0][0], 1);
  EXPECT_EQ(to[1][0], 2);
  EXPECT_EQ(to[2][0], 3);

  EXPECT_EQ(to[0][1], 4);
  EXPECT_EQ(to[1][1], 5);
  EXPECT_EQ(to[2][1], 6);

  EXPECT_EQ(to[0][2], 7);
  EXPECT_EQ(to[1][2], 8);
  EXPECT_EQ(to[2][2], 9);
}

TEST(ColumnViewsAssignment, InitializerListToColumnMajorColumnViews)
{
  maths::matrix<int, 3, 3, maths::layout::column_major> to;

  to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
  to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
  to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

  to.columns() = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
  };

  EXPECT_EQ(to[0][0], 1);
  EXPECT_EQ(to[1][0], 2);
  EXPECT_EQ(to[2][0], 3);

  EXPECT_EQ(to[0][1], 4);
  EXPECT_EQ(to[1][1], 5);
  EXPECT_EQ(to[2][1], 6);

  EXPECT_EQ(to[0][2], 7);
  EXPECT_EQ(to[1][2], 8);
  EXPECT_EQ(to[2][2], 9);
}
}