#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(ColumnViewConstruction, RowMajorRowView)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> mat;

    mat[0][0] = 1, mat[0][1] = 4;
    mat[1][0] = 2, mat[1][1] = 5;
    mat[2][0] = 3, mat[2][1] = 6;

    return mat;
  }();

  static_assert(mat.column(0)[0] == 1);
  static_assert(mat.column(0)[1] == 2);
  static_assert(mat.column(0)[2] == 3);

  static_assert(mat.column(1)[0] == 4);
  static_assert(mat.column(1)[1] == 5);
  static_assert(mat.column(1)[2] == 6);
}

TEST(ColumnViewConstruction, ColumnMajorRowView)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> mat;

    mat[0][0] = 1, mat[0][1] = 4;
    mat[1][0] = 2, mat[1][1] = 5;
    mat[2][0] = 3, mat[2][1] = 6;

    return mat;
  }();

  static_assert(mat.column(0)[0] == 1);
  static_assert(mat.column(0)[1] == 2);
  static_assert(mat.column(0)[2] == 3);

  static_assert(mat.column(1)[0] == 4);
  static_assert(mat.column(1)[1] == 5);
  static_assert(mat.column(1)[2] == 6);
}
}