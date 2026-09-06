#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(ColumnViewsConstruction, RowMajorRowView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> mat;

  mat[0][0] = 1, mat[0][1] = 4;
  mat[1][0] = 2, mat[1][1] = 5;
  mat[2][0] = 3, mat[2][1] = 6;

  EXPECT_EQ(mat.columns().size(), mat.row_size());

  EXPECT_EQ(mat.columns()[0], mat.column(0));
  EXPECT_EQ(mat.columns()[1], mat.column(1));
}

TEST(ColumnViewsConstruction, ColumnMajorRowView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> mat;

  mat[0][0] = 1, mat[0][1] = 4;
  mat[1][0] = 2, mat[1][1] = 5;
  mat[2][0] = 3, mat[2][1] = 6;

  EXPECT_EQ(mat.columns().size(), mat.row_size());

  EXPECT_EQ(mat.columns()[0], mat.column(0));
  EXPECT_EQ(mat.columns()[1], mat.column(1));
}
}