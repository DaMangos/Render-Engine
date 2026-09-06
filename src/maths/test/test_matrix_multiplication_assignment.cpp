#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>
#include <utility>

namespace
{
TEST(MatrixMultiplicationAssignment, RowMajorMatrixTimesRowMajorMatrix)
{
  maths::matrix<int, 2, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;

  maths::matrix<int, 2, 2, maths::layout::row_major> y;

  y[0][0] = 5, y[0][1] = 6;
  y[1][0] = 7, y[1][1] = 8;

  auto & result = (x *= y);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::row_major> &>);

  EXPECT_EQ(result[0][0], 19);
  EXPECT_EQ(result[0][1], 22);

  EXPECT_EQ(result[1][0], 43);
  EXPECT_EQ(result[1][1], 50);
}

TEST(MatrixMultiplicationAssignment, ColumnMajorMatrixTimesRowMajorMatrix)
{
  maths::matrix<int, 2, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;

  maths::matrix<int, 2, 2, maths::layout::row_major> y;

  y[0][0] = 5, y[0][1] = 6;
  y[1][0] = 7, y[1][1] = 8;

  auto & result = (x *= y);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::column_major> &>);

  EXPECT_EQ(result[0][0], 19);
  EXPECT_EQ(result[0][1], 22);

  EXPECT_EQ(result[1][0], 43);
  EXPECT_EQ(result[1][1], 50);
}

TEST(MatrixMultiplicationAssignment, RowMajorMatrixTimesColumnMajorMatrix)
{
  maths::matrix<int, 2, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;

  maths::matrix<int, 2, 2, maths::layout::column_major> y;

  y[0][0] = 5, y[0][1] = 6;
  y[1][0] = 7, y[1][1] = 8;

  auto & result = (x *= y);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::row_major> &>);

  EXPECT_EQ(result[0][0], 19);
  EXPECT_EQ(result[0][1], 22);

  EXPECT_EQ(result[1][0], 43);
  EXPECT_EQ(result[1][1], 50);
}

TEST(MatrixMultiplicationAssignment, ColumnMajorMatrixTimesColumnMajorMatrix)
{
  maths::matrix<int, 2, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;

  maths::matrix<int, 2, 2, maths::layout::column_major> y;

  y[0][0] = 5, y[0][1] = 6;
  y[1][0] = 7, y[1][1] = 8;

  auto & result = (x *= y);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::column_major> &>);

  EXPECT_EQ(result[0][0], 19);
  EXPECT_EQ(result[0][1], 22);

  EXPECT_EQ(result[1][0], 43);
  EXPECT_EQ(result[1][1], 50);
}

TEST(MatrixMultiplicationAssignment, RowMajorMatrixTimesScalar)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  auto & result = (x *= 2);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 3, maths::layout::row_major> &>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);

  EXPECT_EQ(result[1][0], 8);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[1][2], 12);
}

TEST(MatrixMultiplicationAssignment, ColumnMajorMatrixTimesScalar)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  auto & result = (x *= 2);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 3, maths::layout::column_major> &>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);

  EXPECT_EQ(result[1][0], 8);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[1][2], 12);
}

TEST(MatrixMultiplicationAssignment, RvalueRowMajorMatrixTimesRowMajorMatrix)
{
  maths::matrix<int, 2, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;

  maths::matrix<int, 2, 2, maths::layout::row_major> y;

  y[0][0] = 5, y[0][1] = 6;
  y[1][0] = 7, y[1][1] = 8;

  auto result = (std::move(x) *= y);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 19);
  EXPECT_EQ(result[0][1], 22);

  EXPECT_EQ(result[1][0], 43);
  EXPECT_EQ(result[1][1], 50);
}

TEST(MatrixMultiplicationAssignment, RvalueColumnMajorMatrixTimesRowMajorMatrix)
{
  maths::matrix<int, 2, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;

  maths::matrix<int, 2, 2, maths::layout::row_major> y;

  y[0][0] = 5, y[0][1] = 6;
  y[1][0] = 7, y[1][1] = 8;

  auto result = (std::move(x) *= y);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 19);
  EXPECT_EQ(result[0][1], 22);

  EXPECT_EQ(result[1][0], 43);
  EXPECT_EQ(result[1][1], 50);
}

TEST(MatrixMultiplicationAssignment, RvalueRowMajorMatrixTimesColumnMajorMatrix)
{
  maths::matrix<int, 2, 2, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;

  maths::matrix<int, 2, 2, maths::layout::column_major> y;

  y[0][0] = 5, y[0][1] = 6;
  y[1][0] = 7, y[1][1] = 8;

  auto result = (std::move(x) *= y);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 19);
  EXPECT_EQ(result[0][1], 22);

  EXPECT_EQ(result[1][0], 43);
  EXPECT_EQ(result[1][1], 50);
}

TEST(MatrixMultiplicationAssignment, RvalueColumnMajorMatrixTimesColumnMajorMatrix)
{
  maths::matrix<int, 2, 2, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2;
  x[1][0] = 3, x[1][1] = 4;

  maths::matrix<int, 2, 2, maths::layout::column_major> y;

  y[0][0] = 5, y[0][1] = 6;
  y[1][0] = 7, y[1][1] = 8;

  auto result = (std::move(x) *= y);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 19);
  EXPECT_EQ(result[0][1], 22);

  EXPECT_EQ(result[1][0], 43);
  EXPECT_EQ(result[1][1], 50);
}

TEST(MatrixMultiplicationAssignment, RvalueRowMajorMatrixTimesScalar)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  auto result = (std::move(x) *= 2);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 3, maths::layout::row_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);

  EXPECT_EQ(result[1][0], 8);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[1][2], 12);
}

TEST(MatrixMultiplicationAssignment, RvalueColumnMajorMatrixTimesScalar)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> x;

  x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
  x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

  auto result = (std::move(x) *= 2);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 3, maths::layout::column_major>>);

  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[0][2], 6);

  EXPECT_EQ(result[1][0], 8);
  EXPECT_EQ(result[1][1], 10);
  EXPECT_EQ(result[1][2], 12);
}
}
