#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(RowViewsConstruction, RowMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> mat;

  mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
  mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

  EXPECT_EQ(mat.rows().size(), mat.column_size());

  EXPECT_EQ(mat.rows()[0], mat.row(0));
  EXPECT_EQ(mat.rows()[1], mat.row(1));
}

TEST(RowViewsConstruction, ColumnMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> mat;

  mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
  mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

  EXPECT_EQ(mat.rows().size(), mat.column_size());

  EXPECT_EQ(mat.rows()[0], mat.row(0));
  EXPECT_EQ(mat.rows()[1], mat.row(1));
}
}