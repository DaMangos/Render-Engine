#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(MatrixAddition, RowMajorMatrixAddRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);

  static_assert(result[0][1] == 8);
  static_assert(result[1][1] == 10);
  static_assert(result[2][1] == 12);
}

TEST(MatrixAddition, ColumnMajorMatrixAddRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);

  static_assert(result[0][1] == 8);
  static_assert(result[1][1] == 10);
  static_assert(result[2][1] == 12);
}

TEST(MatrixAddition, RowMajorMatrixAddColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);

  static_assert(result[0][1] == 8);
  static_assert(result[1][1] == 10);
  static_assert(result[2][1] == 12);
}

TEST(MatrixAddition, ColumnMajorMatrixAddColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);

  static_assert(result[0][1] == 8);
  static_assert(result[1][1] == 10);
  static_assert(result[2][1] == 12);
}

TEST(MatrixAddition, RowMajorMatrixAddRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 1, maths::layout::row_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(MatrixAddition, RowMajorMatrixAddColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 1, maths::layout::row_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(MatrixAddition, ColumnMajorMatrixAddRowMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 1, maths::layout::column_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(MatrixAddition, ColumnMajorMatrixAddColumnMajorColumnView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 1, maths::layout::column_major> x;

    x[0][0] = 1;
    x[1][0] = 2;
    x[2][0] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 2, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 4;
    y[1][0] = 2, y[1][1] = 5;
    y[2][0] = 3, y[2][1] = 6;

    return y;
  }();

  constexpr auto result = x + y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(MatrixAddition, RowMajorColumnViewAddRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::row_major> y;

    y[0][0] = 1;
    y[1][0] = 2;
    y[2][0] = 3;

    return y;
  }();

  constexpr auto result = x.column(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(MatrixAddition, RowMajorColumnViewAddColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::column_major> y;

    y[0][0] = 1;
    y[1][0] = 2;
    y[2][0] = 3;

    return y;
  }();

  constexpr auto result = x.column(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(MatrixAddition, ColumnMajorColumnViewAddRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::row_major> y;

    y[0][0] = 1;
    y[1][0] = 2;
    y[2][0] = 3;

    return y;
  }();

  constexpr auto result = x.column(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(MatrixAddition, ColumnMajorColumnViewAddColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 4;
    x[1][0] = 2, x[1][1] = 5;
    x[2][0] = 3, x[2][1] = 6;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 3, 1, maths::layout::column_major> y;

    y[0][0] = 1;
    y[1][0] = 2;
    y[2][0] = 3;

    return y;
  }();

  constexpr auto result = x.column(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[1][0] == 4);
  static_assert(result[2][0] == 6);
}

TEST(MatrixAddition, RowMajorMatrixAddRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  constexpr auto result = x + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);
}

TEST(MatrixAddition, RowMajorMatrixAddColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  constexpr auto result = x + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);
}

TEST(MatrixAddition, ColumnMajorMatrixAddRowMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  constexpr auto result = x + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);
}

TEST(MatrixAddition, ColumnMajorMatrixAddColumnMajorRowView)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;

    return x;
  }();

  constexpr auto y = []()
  {
    maths::matrix<long, 2, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;
    y[1][0] = 4, y[1][1] = 5, y[1][2] = 6;

    return y;
  }();

  constexpr auto result = x + y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);
}

TEST(MatrixAddition, RowMajorRowViewAddRowMajorMatrix)
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
    maths::matrix<long, 1, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;

    return y;
  }();

  constexpr auto result = x.row(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);
}

TEST(MatrixAddition, RowMajorRowViewAddColumnMajorMatrix)
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
    maths::matrix<long, 1, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;

    return y;
  }();

  constexpr auto result = x.row(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);
}

TEST(MatrixAddition, ColumnMajorRowViewAddRowMajorMatrix)
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
    maths::matrix<long, 1, 3, maths::layout::row_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;

    return y;
  }();

  constexpr auto result = x.row(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);
}

TEST(MatrixAddition, ColumnMajorRowViewAddColumnMajorMatrix)
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
    maths::matrix<long, 1, 3, maths::layout::column_major> y;

    y[0][0] = 1, y[0][1] = 2, y[0][2] = 3;

    return y;
  }();

  constexpr auto result = x.row(0) + y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);
}
}