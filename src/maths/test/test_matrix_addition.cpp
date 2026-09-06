#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(MatrixAddition, RowMajorMatrixAddRowMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);

  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[2][1], 12);
}

TEST(MatrixAddition, ColumnMajorMatrixAddRowMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);

  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[2][1], 12);
}

TEST(MatrixAddition, RowMajorMatrixAddColumnMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);

  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[2][1], 12);
}

TEST(MatrixAddition, ColumnMajorMatrixAddColumnMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);

  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[2][1], 12);
}

TEST(MatrixAddition, RowMajorMatrixAddRowMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::row_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(MatrixAddition, RowMajorMatrixAddColumnMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::row_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(MatrixAddition, ColumnMajorMatrixAddRowMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::column_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(MatrixAddition, ColumnMajorMatrixAddColumnMajorColumnView)
{
  maths::matrix<int, 3, 1, maths::layout::column_major> x;

  x[0][0] = 1;
  x[1][0] = 2;
  x[2][0] = 3;

  maths::matrix<long, 3, 2, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 4;
  y[1][0] = 2, y[1][1] = 5;
  y[2][0] = 3, y[2][1] = 6;

  auto result = x + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(MatrixAddition, RowMajorColumnViewAddRowMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 1, maths::layout::row_major> y;

  y[0][0] = 1;
  y[1][0] = 2;
  y[2][0] = 3;

  auto result = x.column(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(MatrixAddition, RowMajorColumnViewAddColumnMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 1, maths::layout::column_major> y;

  y[0][0] = 1;
  y[1][0] = 2;
  y[2][0] = 3;

  auto result = x.column(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(MatrixAddition, ColumnMajorColumnViewAddRowMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 1, maths::layout::row_major> y;

  y[0][0] = 1;
  y[1][0] = 2;
  y[2][0] = 3;

  auto result = x.column(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(MatrixAddition, ColumnMajorColumnViewAddColumnMajorMatrix)
{
  maths::matrix<int, 3, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 4;
  x[1][0] = 2, x[1][1] = 5;
  x[2][0] = 3, x[2][1] = 6;

  maths::matrix<long, 3, 1, maths::layout::column_major> y;

  y[0][0] = 1;
  y[1][0] = 2;
  y[2][0] = 3;

  auto result = x.column(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[1][0], 4);
  EXPECT_EQ(result[2][0], 6);
}

TEST(MatrixAddition, RowMajorMatrixAddRowMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  auto result = x + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
}

TEST(MatrixAddition, RowMajorMatrixAddColumnMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  auto result = x + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
}

TEST(MatrixAddition, ColumnMajorMatrixAddRowMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  auto result = x + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
}

TEST(MatrixAddition, ColumnMajorMatrixAddColumnMajorRowView)
{
  maths::matrix<int, 1, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

  maths::matrix<long, 2, 3, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
  y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

  auto result = x + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
}

TEST(MatrixAddition, RowMajorRowViewAddRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 1, 3, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;

  auto result = x.row(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
}

TEST(MatrixAddition, RowMajorRowViewAddColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 1, 3, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;

  auto result = x.row(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
}

TEST(MatrixAddition, ColumnMajorRowViewAddRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 1, 3, maths::layout::row_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;

  auto result = x.row(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
}

TEST(MatrixAddition, ColumnMajorRowViewAddColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  maths::matrix<long, 1, 3, maths::layout::column_major> y;

  y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;

  auto result = x.row(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);
}
}