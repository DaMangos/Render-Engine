#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(RowViewsEquality, RowMajorMatrixCompareRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

    return x;
  }();

  constexpr maths::matrix<long, 3, 3, maths::layout::row_major> y;

  static_assert(x.rows() == x.rows());
  static_assert(x.rows() != y.rows());
  static_assert(y.rows() != x.rows());
  static_assert(y.rows() == y.rows());
}

TEST(RowViewsEquality, ColumnMajorMatrixCompareRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4, x[0][2] = 7;
    x[1][0] = 2, x[1][1] = 5, x[1][2] = 8;
    x[2][0] = 3, x[2][1] = 6, x[2][2] = 9;

    return x;
  }();

  constexpr maths::matrix<long, 3, 3, maths::layout::row_major> y;

  static_assert(x.rows() == x.rows());
  static_assert(x.rows() != y.rows());
  static_assert(y.rows() != x.rows());
  static_assert(y.rows() == y.rows());
}
}