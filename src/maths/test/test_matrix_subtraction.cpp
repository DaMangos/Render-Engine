#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(MatrixSubtraction, RowMajorMatrixMinusRowMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 8;
  y[1][0] = 4, y[1][1] = 10;
  y[2][0] = 6, y[2][1] = 12;

  auto result = x - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);

  EXPECT_EQ(result[0][1], -4);
  EXPECT_EQ(result[1][1], -5);
  EXPECT_EQ(result[2][1], -6);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusRowMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 8;
  y[1][0] = 4, y[1][1] = 10;
  y[2][0] = 6, y[2][1] = 12;

  auto result = x - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);

  EXPECT_EQ(result[0][1], -4);
  EXPECT_EQ(result[1][1], -5);
  EXPECT_EQ(result[2][1], -6);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusColumnMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 8;
  y[1][0] = 4, y[1][1] = 10;
  y[2][0] = 6, y[2][1] = 12;

  auto result = x - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);

  EXPECT_EQ(result[0][1], -4);
  EXPECT_EQ(result[1][1], -5);
  EXPECT_EQ(result[2][1], -6);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusColumnMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 8;
  y[1][0] = 4, y[1][1] = 10;
  y[2][0] = 6, y[2][1] = 12;

  auto result = x - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);

  EXPECT_EQ(result[0][1], -4);
  EXPECT_EQ(result[1][1], -5);
  EXPECT_EQ(result[2][1], -6);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusRowMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::row_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 8;
  y[1][0] = 4, y[1][1] = 10;
  y[2][0] = 6, y[2][1] = 12;

  auto result = x - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusColumnMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::row_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 8;
  y[1][0] = 4, y[1][1] = 10;
  y[2][0] = 6, y[2][1] = 12;

  auto result = x - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusRowMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::column_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 8;
  y[1][0] = 4, y[1][1] = 10;
  y[2][0] = 6, y[2][1] = 12;

  auto result = x - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusColumnMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::column_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 8;
  y[1][0] = 4, y[1][1] = 10;
  y[2][0] = 6, y[2][1] = 12;

  auto result = x - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);
}

TEST(MatrixSubtraction, RowMajorColumnViewMinusRowMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 1, maths::layout::row_major> y;

  y[0][0] = 2;
  y[1][0] = 4;
  y[2][0] = 6;

  auto result = x.column(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);
}

TEST(MatrixSubtraction, RowMajorColumnViewMinusColumnMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 1, maths::layout::column_major> y;

  y[0][0] = 2;
  y[1][0] = 4;
  y[2][0] = 6;

  auto result = x.column(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);
}

TEST(MatrixSubtraction, ColumnMajorColumnViewMinusRowMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 1, maths::layout::row_major> y;

  y[0][0] = 2;
  y[1][0] = 4;
  y[2][0] = 6;

  auto result = x.column(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);
}

TEST(MatrixSubtraction, ColumnMajorColumnViewMinusColumnMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 1, maths::layout::column_major> y;

  y[0][0] = 2;
  y[1][0] = 4;
  y[2][0] = 6;

  auto result = x.column(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[1][0], -2);
  EXPECT_EQ(result[2][0], -3);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusRowMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
  y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

  auto result = x - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusColumnMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
  y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

  auto result = x - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusRowMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
  y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

  auto result = x - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusColumnMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
  y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

  auto result = x - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(MatrixSubtraction, RowMajorRowViewMinusRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 1, 3, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;

  auto result = x.row(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(MatrixSubtraction, RowMajorRowViewMinusColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 1, 3, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;

  auto result = x.row(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(MatrixSubtraction, ColumnMajorRowViewMinusRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 1, 3, maths::layout::row_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;

  auto result = x.row(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}

TEST(MatrixSubtraction, ColumnMajorRowViewMinusColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 1, 3, maths::layout::column_major> y;

  y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;

  auto result = x.row(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], -1);
  EXPECT_EQ(result[0][1], -2);
  EXPECT_EQ(result[0][2], -3);
}
}