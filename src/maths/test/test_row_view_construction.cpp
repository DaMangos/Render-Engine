#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(RowViewConstruction, RowMajorRowView)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> mat;

    mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
    mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

    return mat;
  }();

  static_assert(mat.row(0)[0] == 1);
  static_assert(mat.row(0)[1] == 2);
  static_assert(mat.row(0)[2] == 3);

  static_assert(mat.row(1)[0] == 4);
  static_assert(mat.row(1)[1] == 5);
  static_assert(mat.row(1)[2] == 6);
}

TEST(RowViewConstruction, ColumnMajorRowView)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> mat;

    mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
    mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

    return mat;
  }();

  static_assert(mat.row(0)[0] == 1);
  static_assert(mat.row(0)[1] == 2);
  static_assert(mat.row(0)[2] == 3);

  static_assert(mat.row(1)[0] == 4);
  static_assert(mat.row(1)[1] == 5);
  static_assert(mat.row(1)[2] == 6);
}

TEST(RowViewConstruction, RowMajorOperatorSubscript)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> mat;

    mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
    mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

    return mat;
  }();

  static_assert(mat[0][0] == mat.row(0)[0]);
  static_assert(mat[0][1] == mat.row(0)[1]);
  static_assert(mat[0][2] == mat.row(0)[2]);

  static_assert(mat[1][0] == mat.row(1)[0]);
  static_assert(mat[1][1] == mat.row(1)[1]);
  static_assert(mat[1][2] == mat.row(1)[2]);
}

TEST(RowViewConstruction, ColumnMajorOperatorSubscript)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> mat;

    mat[0][0] = 1, mat[0][1] = 2, mat[0][2] = 3;
    mat[1][0] = 4, mat[1][1] = 5, mat[1][2] = 6;

    return mat;
  }();

  static_assert(mat[0][0] == mat.row(0)[0]);
  static_assert(mat[0][1] == mat.row(0)[1]);
  static_assert(mat[0][2] == mat.row(0)[2]);

  static_assert(mat[1][0] == mat.row(1)[0]);
  static_assert(mat[1][1] == mat.row(1)[1]);
  static_assert(mat[1][2] == mat.row(1)[2]);
}
}