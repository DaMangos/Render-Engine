#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(MatrixMultiplication, RowMajorMatrixTimesRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 7, y[0][1] = 8;
  y[1][0] = 9, y[1][1] = 10;
  y[2][0] = 11, y[2][1] = 12;

  auto result = x * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 2, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 58);
  EXPECT_EQ(result[1][0], 139);

  EXPECT_EQ(result[0][1], 64);
  EXPECT_EQ(result[1][1], 154);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 7, y[0][1] = 8;
  y[1][0] = 9, y[1][1] = 10;
  y[2][0] = 11, y[2][1] = 12;

  auto result = x * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 2, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 58);
  EXPECT_EQ(result[1][0], 139);

  EXPECT_EQ(result[0][1], 64);
  EXPECT_EQ(result[1][1], 154);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 7, y[0][1] = 8;
  y[1][0] = 9, y[1][1] = 10;
  y[2][0] = 11, y[2][1] = 12;

  auto result = x * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 2, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 58);
  EXPECT_EQ(result[1][0], 139);

  EXPECT_EQ(result[0][1], 64);
  EXPECT_EQ(result[1][1], 154);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 7, y[0][1] = 8;
  y[1][0] = 9, y[1][1] = 10;
  y[2][0] = 11, y[2][1] = 12;

  auto result = x * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 2, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 58);
  EXPECT_EQ(result[1][0], 139);

  EXPECT_EQ(result[0][1], 64);
  EXPECT_EQ(result[1][1], 154);
}

TEST(MatrixMultiplication, RowMajorRowMatrixTimesRowMajorColumnMatrix)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 3, 1, maths::layout::row_major> y;

  y[0][0] = 4;
  y[1][0] = 5;
  y[2][0] = 6;

  auto result = x * y;

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 32);
}

TEST(MatrixMultiplication, ColumnMajorRowMatrixTimesRowMajorColumnMatrix)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 3, 1, maths::layout::row_major> y;

  y[0][0] = 4;
  y[1][0] = 5;
  y[2][0] = 6;

  auto result = x * y;

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 32);
}

TEST(MatrixMultiplication, RowMajorRowMatrixTimesColumnMajorColumnMatrix)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 3, 1, maths::layout::column_major> y;

  y[0][0] = 4;
  y[1][0] = 5;
  y[2][0] = 6;

  auto result = x * y;

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 32);
}

TEST(MatrixMultiplication, ColumnMajorRowMatrixTimesColumnMajorColumnMatrix)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 3, 1, maths::layout::column_major> y;

  y[0][0] = 4;
  y[1][0] = 5;
  y[2][0] = 6;

  auto result = x * y;

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 32);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesRowMajorColumnView)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 4, y[0][1] = 7;
  y[1][0] = 5, y[1][1] = 8;
  y[2][0] = 6, y[2][1] = 9;

  auto result = x * y.column(0);

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 32);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesRowMajorColumnView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 4, y[0][1] = 7;
  y[1][0] = 5, y[1][1] = 8;
  y[2][0] = 6, y[2][1] = 9;

  auto result = x * y.column(0);

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 32);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesColumnMajorColumnView)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 4, y[0][1] = 7;
  y[1][0] = 5, y[1][1] = 8;
  y[2][0] = 6, y[2][1] = 9;

  auto result = x * y.column(0);

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 32);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesColumnMajorColumnView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 4, y[0][1] = 7;
  y[1][0] = 5, y[1][1] = 8;
  y[2][0] = 6, y[2][1] = 9;

  auto result = x * y.column(0);

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 32);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesRowMajorMatrix)
{
  maths::matrix<long, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<int, 1, 2, maths::layout::row_major> y;

  y[0][0] = 7, y[0][1] = 8;

  auto result = x.column(0) * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 7);
  EXPECT_EQ(result[1][0], 14);
  EXPECT_EQ(result[2][0], 21);

  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][1], 16);
  EXPECT_EQ(result[2][1], 24);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesRowMajorMatrix)
{
  maths::matrix<long, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<int, 1, 2, maths::layout::row_major> y;

  y[0][0] = 7, y[0][1] = 8;

  auto result = x.column(0) * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 7);
  EXPECT_EQ(result[1][0], 14);
  EXPECT_EQ(result[2][0], 21);

  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][1], 16);
  EXPECT_EQ(result[2][1], 24);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesColumnMajorMatrix)
{
  maths::matrix<long, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<int, 1, 2, maths::layout::column_major> y;

  y[0][0] = 7, y[0][1] = 8;

  auto result = x.column(0) * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 7);
  EXPECT_EQ(result[1][0], 14);
  EXPECT_EQ(result[2][0], 21);

  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][1], 16);
  EXPECT_EQ(result[2][1], 24);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesColumnMajorMatrix)
{
  maths::matrix<long, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<int, 1, 2, maths::layout::column_major> y;

  y[0][0] = 7, y[0][1] = 8;

  auto result = x.column(0) * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 7);
  EXPECT_EQ(result[1][0], 14);
  EXPECT_EQ(result[2][0], 21);

  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][1], 16);
  EXPECT_EQ(result[2][1], 24);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesRowMajorRowView)
{
  maths::matrix<long, 3, 1, maths::layout::row_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<int, 2, 2, maths::layout::row_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 4);
  EXPECT_EQ(result[1][0], 8);
  EXPECT_EQ(result[2][0], 12);

  EXPECT_EQ(result[0][1], 5);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[2][1], 15);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesRowMajorRowView)
{
  maths::matrix<long, 3, 1, maths::layout::column_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<int, 2, 2, maths::layout::row_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 4);
  EXPECT_EQ(result[1][0], 8);
  EXPECT_EQ(result[2][0], 12);

  EXPECT_EQ(result[0][1], 5);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[2][1], 15);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesColumnMajorRowView)
{
  maths::matrix<long, 3, 1, maths::layout::row_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<int, 2, 2, maths::layout::column_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 4);
  EXPECT_EQ(result[1][0], 8);
  EXPECT_EQ(result[2][0], 12);

  EXPECT_EQ(result[0][1], 5);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[2][1], 15);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesColumnMajorRowView)
{
  maths::matrix<long, 3, 1, maths::layout::column_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<int, 2, 2, maths::layout::column_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 4);
  EXPECT_EQ(result[1][0], 8);
  EXPECT_EQ(result[2][0], 12);

  EXPECT_EQ(result[0][1], 5);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[2][1], 15);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 3, 1, maths::layout::row_major> y;

  y[0][0] = 7;
  y[1][0] = 8;
  y[2][0] = 9;

  auto result = x.row(0) * y;

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 50);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 3, 1, maths::layout::row_major> y;

  y[0][0] = 7;
  y[1][0] = 8;
  y[2][0] = 9;

  auto result = x.row(0) * y;

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 50);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 3, 1, maths::layout::column_major> y;

  y[0][0] = 7;
  y[1][0] = 8;
  y[2][0] = 9;

  auto result = x.row(0) * y;

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 50);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 3, 1, maths::layout::column_major> y;

  y[0][0] = 7;
  y[1][0] = 8;
  y[2][0] = 9;

  auto result = x.row(0) * y;

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 50);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesRowMajorRowView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  maths::matrix<long, 2, 2, maths::layout::row_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x.column(0) * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 4);
  EXPECT_EQ(result[1][0], 12);
  EXPECT_EQ(result[2][0], 20);

  EXPECT_EQ(result[0][1], 5);
  EXPECT_EQ(result[1][1], 15);
  EXPECT_EQ(result[2][1], 25);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesRowMajorRowView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  maths::matrix<long, 2, 2, maths::layout::row_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x.column(0) * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 4);
  EXPECT_EQ(result[1][0], 12);
  EXPECT_EQ(result[2][0], 20);

  EXPECT_EQ(result[0][1], 5);
  EXPECT_EQ(result[1][1], 15);
  EXPECT_EQ(result[2][1], 25);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesColumnMajorRowView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  maths::matrix<long, 2, 2, maths::layout::column_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x.column(0) * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 4);
  EXPECT_EQ(result[1][0], 12);
  EXPECT_EQ(result[2][0], 20);

  EXPECT_EQ(result[0][1], 5);
  EXPECT_EQ(result[1][1], 15);
  EXPECT_EQ(result[2][1], 25);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesColumnMajorRowView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  maths::matrix<long, 2, 2, maths::layout::column_major> y;

  y[0][0] = 7, y[0][1] = 8;
  y[1][0] = 9, y[1][1] = 10;

  auto result = x.column(0) * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 7);
  EXPECT_EQ(result[1][0], 21);
  EXPECT_EQ(result[2][0], 35);

  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][1], 24);
  EXPECT_EQ(result[2][1], 40);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesRowMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  maths::matrix<long, 2, 2, maths::layout::row_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x.row(0) * y.column(0);

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 16);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesRowMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  maths::matrix<long, 2, 2, maths::layout::row_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x.row(0) * y.column(0);

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 16);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesColumnMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  maths::matrix<long, 2, 2, maths::layout::column_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x.row(0) * y.column(0);

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 16);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesColumnMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  maths::matrix<long, 2, 2, maths::layout::column_major> y;

  y[0][0] = 4, y[0][1] = 5;
  y[1][0] = 6, y[1][1] = 7;

  auto result = x.row(0) * y.column(0);

  static_assert(std::same_as<decltype(result), long>);

  EXPECT_EQ(result, 16);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesScalar)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = x * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[2][0], 10);

  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[1][1], 8);
  EXPECT_EQ(result[2][1], 12);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesScalar)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = x * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[2][0], 10);

  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[1][1], 8);
  EXPECT_EQ(result[2][1], 12);
}

TEST(MatrixMultiplication, ScalarTimesColumnMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = 2 * x;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[2][0], 10);

  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[1][1], 8);
  EXPECT_EQ(result[2][1], 12);
}

TEST(MatrixMultiplication, ScalarTimesRowMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = 2 * x;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[2][0], 10);

  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[1][1], 8);
  EXPECT_EQ(result[2][1], 12);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesScalar)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = x.row(0) * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 1, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesScalar)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = x.row(0) * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 1, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
}

TEST(MatrixMultiplication, ScalarTimesColumnMajorRowView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = 2 * x.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 1, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
}

TEST(MatrixMultiplication, ScalarTimesRowMajorRowView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = 2 * x.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 1, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesScalar)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = x.column(0) * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[2][0], 10);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesScalar)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = x.column(0) * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[2][0], 10);
}

TEST(MatrixMultiplication, ScalarTimesColumnMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = 2 * x.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[2][0], 10);
}

TEST(MatrixMultiplication, ScalarTimesRowMajorColumnView)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;
  x[2][0] = 5, x[2][1] = 6;

  auto result = 2 * x.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[2][0], 10);
}
}
