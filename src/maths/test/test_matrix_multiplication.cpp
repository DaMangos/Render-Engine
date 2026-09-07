#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(MatrixMultiplication, RowMajorMatrixTimesRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 7, y[0][1] = 8;
    y[1][0] = 9, y[1][1] = 10;
    y[2][0] = 11, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 2, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 58);
  static_assert(result[1][0] == 139);

  static_assert(result[0][1] == 64);
  static_assert(result[1][1] == 154);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 7, y[0][1] = 8;
    y[1][0] = 9, y[1][1] = 10;
    y[2][0] = 11, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 2, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 58);
  static_assert(result[1][0] == 139);

  static_assert(result[0][1] == 64);
  static_assert(result[1][1] == 154);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 7, y[0][1] = 8;
    y[1][0] = 9, y[1][1] = 10;
    y[2][0] = 11, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 2, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 58);
  static_assert(result[1][0] == 139);

  static_assert(result[0][1] == 64);
  static_assert(result[1][1] == 154);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 7, y[0][1] = 8;
    y[1][0] = 9, y[1][1] = 10;
    y[2][0] = 11, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 2, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 58);
  static_assert(result[1][0] == 139);

  static_assert(result[0][1] == 64);
  static_assert(result[1][1] == 154);
}

TEST(MatrixMultiplication, RowMajorRowMatrixTimesRowMajorColumnMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::row_major> y;

    y[0][0] = 4;
    y[1][0] = 5;
    y[2][0] = 6;

    return y;
  }();

  constexpr auto result = x * y;

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 32);
}

TEST(MatrixMultiplication, ColumnMajorRowMatrixTimesRowMajorColumnMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::row_major> y;

    y[0][0] = 4;
    y[1][0] = 5;
    y[2][0] = 6;

    return y;
  }();

  constexpr auto result = x * y;

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 32);
}

TEST(MatrixMultiplication, RowMajorRowMatrixTimesColumnMajorColumnMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::column_major> y;

    y[0][0] = 4;
    y[1][0] = 5;
    y[2][0] = 6;

    return y;
  }();

  constexpr auto result = x * y;

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 32);
}

TEST(MatrixMultiplication, ColumnMajorRowMatrixTimesColumnMajorColumnMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::column_major> y;

    y[0][0] = 4;
    y[1][0] = 5;
    y[2][0] = 6;

    return y;
  }();

  constexpr auto result = x * y;

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 32);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 4, y[0][1] = 7;
    y[1][0] = 5, y[1][1] = 8;
    y[2][0] = 6, y[2][1] = 9;

    return y;
  }();

  constexpr auto result = x * y.column(0);

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 32);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 4, y[0][1] = 7;
    y[1][0] = 5, y[1][1] = 8;
    y[2][0] = 6, y[2][1] = 9;

    return y;
  }();

  constexpr auto result = x * y.column(0);

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 32);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 4, y[0][1] = 7;
    y[1][0] = 5, y[1][1] = 8;
    y[2][0] = 6, y[2][1] = 9;

    return y;
  }();

  constexpr auto result = x * y.column(0);

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 32);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 4, y[0][1] = 7;
    y[1][0] = 5, y[1][1] = 8;
    y[2][0] = 6, y[2][1] = 9;

    return y;
  }();

  constexpr auto result = x * y.column(0);

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 32);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<int, 1, 2, maths::layout::row_major> y;

    y[0][0] = 7, y[0][1] = 8;

    return y;
  }();

  constexpr auto result = x.column(0) * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 7);
  static_assert(result[1][0] == 14);
  static_assert(result[2][0] == 21);

  static_assert(result[0][1] == 8);
  static_assert(result[1][1] == 16);
  static_assert(result[2][1] == 24);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<int, 1, 2, maths::layout::row_major> y;

    y[0][0] = 7, y[0][1] = 8;

    return y;
  }();

  constexpr auto result = x.column(0) * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 7);
  static_assert(result[1][0] == 14);
  static_assert(result[2][0] == 21);

  static_assert(result[0][1] == 8);
  static_assert(result[1][1] == 16);
  static_assert(result[2][1] == 24);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<int, 1, 2, maths::layout::column_major> y;

    y[0][0] = 7, y[0][1] = 8;

    return y;
  }();

  constexpr auto result = x.column(0) * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 7);
  static_assert(result[1][0] == 14);
  static_assert(result[2][0] == 21);

  static_assert(result[0][1] == 8);
  static_assert(result[1][1] == 16);
  static_assert(result[2][1] == 24);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<int, 1, 2, maths::layout::column_major> y;

    y[0][0] = 7, y[0][1] = 8;

    return y;
  }();

  constexpr auto result = x.column(0) * y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 7);
  static_assert(result[1][0] == 14);
  static_assert(result[2][0] == 21);

  static_assert(result[0][1] == 8);
  static_assert(result[1][1] == 16);
  static_assert(result[2][1] == 24);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<long, 3, 1, maths::layout::row_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<int, 2, 2, maths::layout::row_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 4);
  static_assert(result[1][0] == 8);
  static_assert(result[2][0] == 12);

  static_assert(result[0][1] == 5);
  static_assert(result[1][1] == 10);
  static_assert(result[2][1] == 15);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<long, 3, 1, maths::layout::column_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<int, 2, 2, maths::layout::row_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 4);
  static_assert(result[1][0] == 8);
  static_assert(result[2][0] == 12);

  static_assert(result[0][1] == 5);
  static_assert(result[1][1] == 10);
  static_assert(result[2][1] == 15);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<long, 3, 1, maths::layout::row_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<int, 2, 2, maths::layout::column_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 4);
  static_assert(result[1][0] == 8);
  static_assert(result[2][0] == 12);

  static_assert(result[0][1] == 5);
  static_assert(result[1][1] == 10);
  static_assert(result[2][1] == 15);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<long, 3, 1, maths::layout::column_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<int, 2, 2, maths::layout::column_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 4);
  static_assert(result[1][0] == 8);
  static_assert(result[2][0] == 12);

  static_assert(result[0][1] == 5);
  static_assert(result[1][1] == 10);
  static_assert(result[2][1] == 15);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::row_major> y;

    y[0][0] = 7;
    y[1][0] = 8;
    y[2][0] = 9;

    return y;
  }();

  constexpr auto result = x.row(0) * y;

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 50);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::row_major> y;

    y[0][0] = 7;
    y[1][0] = 8;
    y[2][0] = 9;

    return y;
  }();

  constexpr auto result = x.row(0) * y;

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 50);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::column_major> y;

    y[0][0] = 7;
    y[1][0] = 8;
    y[2][0] = 9;

    return y;
  }();

  constexpr auto result = x.row(0) * y;

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 50);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::column_major> y;

    y[0][0] = 7;
    y[1][0] = 8;
    y[2][0] = 9;

    return y;
  }();

  constexpr auto result = x.row(0) * y;

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 50);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 2, maths::layout::row_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x.column(0) * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 4);
  static_assert(result[1][0] == 12);
  static_assert(result[2][0] == 20);

  static_assert(result[0][1] == 5);
  static_assert(result[1][1] == 15);
  static_assert(result[2][1] == 25);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 2, maths::layout::row_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x.column(0) * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 4);
  static_assert(result[1][0] == 12);
  static_assert(result[2][0] == 20);

  static_assert(result[0][1] == 5);
  static_assert(result[1][1] == 15);
  static_assert(result[2][1] == 25);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 2, maths::layout::column_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x.column(0) * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 4);
  static_assert(result[1][0] == 12);
  static_assert(result[2][0] == 20);

  static_assert(result[0][1] == 5);
  static_assert(result[1][1] == 15);
  static_assert(result[2][1] == 25);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 2, maths::layout::column_major> y;

    y[0][0] = 7, y[0][1] = 8;
    y[1][0] = 9, y[1][1] = 10;

    return y;
  }();

  constexpr auto result = x.column(0) * y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 7);
  static_assert(result[1][0] == 21);
  static_assert(result[2][0] == 35);

  static_assert(result[0][1] == 8);
  static_assert(result[1][1] == 24);
  static_assert(result[2][1] == 40);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 2, maths::layout::row_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x.row(0) * y.column(0);

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 16);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 2, maths::layout::row_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x.row(0) * y.column(0);

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 16);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 2, maths::layout::column_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x.row(0) * y.column(0);

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 16);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 2, maths::layout::column_major> y;

    y[0][0] = 4, y[0][1] = 5;
    y[1][0] = 6, y[1][1] = 7;

    return y;
  }();

  constexpr auto result = x.row(0) * y.column(0);

  static_assert(std::same_as<decltype(result), long const>);

  static_assert(result == 16);
}

TEST(MatrixMultiplication, ColumnMajorMatrixTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = x * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 6);
  static_assert(result[2][0] == 10);

  static_assert(result[0][1] == 4);
  static_assert(result[1][1] == 8);
  static_assert(result[2][1] == 12);
}

TEST(MatrixMultiplication, RowMajorMatrixTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = x * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 6);
  static_assert(result[2][0] == 10);

  static_assert(result[0][1] == 4);
  static_assert(result[1][1] == 8);
  static_assert(result[2][1] == 12);
}

TEST(MatrixMultiplication, ScalarTimesColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = 2 * x;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 6);
  static_assert(result[2][0] == 10);

  static_assert(result[0][1] == 4);
  static_assert(result[1][1] == 8);
  static_assert(result[2][1] == 12);
}

TEST(MatrixMultiplication, ScalarTimesRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = 2 * x;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 6);
  static_assert(result[2][0] == 10);

  static_assert(result[0][1] == 4);
  static_assert(result[1][1] == 8);
  static_assert(result[2][1] == 12);
}

TEST(MatrixMultiplication, ColumnMajorRowViewTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = x.row(0) * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 1, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
}

TEST(MatrixMultiplication, RowMajorRowViewTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = x.row(0) * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 1, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
}

TEST(MatrixMultiplication, ScalarTimesColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = 2 * x.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 1, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
}

TEST(MatrixMultiplication, ScalarTimesRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = 2 * x.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 1, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
}

TEST(MatrixMultiplication, ColumnMajorColumnViewTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = x.column(0) * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 6);
  static_assert(result[2][0] == 10);
}

TEST(MatrixMultiplication, RowMajorColumnViewTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = x.column(0) * 2;

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 6);
  static_assert(result[2][0] == 10);
}

TEST(MatrixMultiplication, ScalarTimesColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = 2 * x.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 6);
  static_assert(result[2][0] == 10);
}

TEST(MatrixMultiplication, ScalarTimesRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;
    x[2][0] = 5, x[2][1] = 6;

    return x;
  }();

  constexpr auto result = 2 * x.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<int, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 6);
  static_assert(result[2][0] == 10);
}
}
