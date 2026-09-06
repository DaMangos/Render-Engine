#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(RowViewConstruction, RowMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> mat;

  mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
  mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

  EXPECT_EQ(mat.row(0)[0], 1);
  EXPECT_EQ(mat.row(0)[1], 2);
  EXPECT_EQ(mat.row(0)[2], 3);

  EXPECT_EQ(mat.row(1)[0], 4);
  EXPECT_EQ(mat.row(1)[1], 5);
  EXPECT_EQ(mat.row(1)[2], 6);
}

TEST(RowViewConstruction, ColumnMajorRowView)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> mat;

  mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
  mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

  EXPECT_EQ(mat.row(0)[0], 1);
  EXPECT_EQ(mat.row(0)[1], 2);
  EXPECT_EQ(mat.row(0)[2], 3);

  EXPECT_EQ(mat.row(1)[0], 4);
  EXPECT_EQ(mat.row(1)[1], 5);
  EXPECT_EQ(mat.row(1)[2], 6);
}

TEST(RowViewConstruction, RowMajorOperatorSubscript)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> mat;

  mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
  mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

  EXPECT_EQ(mat[0][0], mat.row(0)[0]);
  EXPECT_EQ(mat[0][1], mat.row(0)[1]);
  EXPECT_EQ(mat[0][2], mat.row(0)[2]);

  EXPECT_EQ(mat[1][0], mat.row(1)[0]);
  EXPECT_EQ(mat[1][1], mat.row(1)[1]);
  EXPECT_EQ(mat[1][2], mat.row(1)[2]);
}

TEST(RowViewConstruction, ColumnMajorOperatorSubscript)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> mat;

  mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
  mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

  EXPECT_EQ(mat[0][0], mat.row(0)[0]);
  EXPECT_EQ(mat[0][1], mat.row(0)[1]);
  EXPECT_EQ(mat[0][2], mat.row(0)[2]);

  EXPECT_EQ(mat[1][0], mat.row(1)[0]);
  EXPECT_EQ(mat[1][1], mat.row(1)[1]);
  EXPECT_EQ(mat[1][2], mat.row(1)[2]);
}
}