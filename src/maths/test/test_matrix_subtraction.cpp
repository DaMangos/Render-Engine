#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>

namespace
{
TEST(MatrixSubtraction, RowMajorMatrixMinusRowMajorMatrix)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);

  static_assert(result[0][1] == -4);
  static_assert(result[1][1] == -5);
  static_assert(result[2][1] == -6);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusRowMajorMatrix)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);

  static_assert(result[0][1] == -4);
  static_assert(result[1][1] == -5);
  static_assert(result[2][1] == -6);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusColumnMajorMatrix)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);

  static_assert(result[0][1] == -4);
  static_assert(result[1][1] == -5);
  static_assert(result[2][1] == -6);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusColumnMajorMatrix)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 2, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);

  static_assert(result[0][1] == -4);
  static_assert(result[1][1] == -5);
  static_assert(result[2][1] == -6);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusRowMajorColumnView)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusColumnMajorColumnView)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusRowMajorColumnView)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusColumnMajorColumnView)
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

    y[0][0] = 2, y[0][1] = 8;
    y[1][0] = 4, y[1][1] = 10;
    y[2][0] = 6, y[2][1] = 12;

    return y;
  }();

  constexpr auto result = x - y.column(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);
}

TEST(MatrixSubtraction, RowMajorColumnViewMinusRowMajorMatrix)
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

    y[0][0] = 2;
    y[1][0] = 4;
    y[2][0] = 6;

    return y;
  }();

  constexpr auto result = x.column(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);
}

TEST(MatrixSubtraction, RowMajorColumnViewMinusColumnMajorMatrix)
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

    y[0][0] = 2;
    y[1][0] = 4;
    y[2][0] = 6;

    return y;
  }();

  constexpr auto result = x.column(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);
}

TEST(MatrixSubtraction, ColumnMajorColumnViewMinusRowMajorMatrix)
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

    y[0][0] = 2;
    y[1][0] = 4;
    y[2][0] = 6;

    return y;
  }();

  constexpr auto result = x.column(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);
}

TEST(MatrixSubtraction, ColumnMajorColumnViewMinusColumnMajorMatrix)
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

    y[0][0] = 2;
    y[1][0] = 4;
    y[2][0] = 6;

    return y;
  }();

  constexpr auto result = x.column(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 3, 1, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[1][0] == -2);
  static_assert(result[2][0] == -3);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusRowMajorRowView)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
    y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

    return y;
  }();

  constexpr auto result = x - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(MatrixSubtraction, RowMajorMatrixMinusColumnMajorRowView)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
    y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

    return y;
  }();

  constexpr auto result = x - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusRowMajorRowView)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
    y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

    return y;
  }();

  constexpr auto result = x - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(MatrixSubtraction, ColumnMajorMatrixMinusColumnMajorRowView)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;
    y[1][0] = 8, y[1][1] = 10, y[1][2] = 12;

    return y;
  }();

  constexpr auto result = x - y.row(0);

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(MatrixSubtraction, RowMajorRowViewMinusRowMajorMatrix)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;

    return y;
  }();

  constexpr auto result = x.row(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(MatrixSubtraction, RowMajorRowViewMinusColumnMajorMatrix)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;

    return y;
  }();

  constexpr auto result = x.row(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::row_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(MatrixSubtraction, ColumnMajorRowViewMinusRowMajorMatrix)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;

    return y;
  }();

  constexpr auto result = x.row(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}

TEST(MatrixSubtraction, ColumnMajorRowViewMinusColumnMajorMatrix)
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

    y[0][0] = 2, y[0][1] = 4, y[0][2] = 6;

    return y;
  }();

  constexpr auto result = x.row(0) - y;

  static_assert(std::same_as<decltype(result), maths::matrix<long, 1, 3, maths::layout::column_major> const>);

  static_assert(result[0][0] == -1);
  static_assert(result[0][1] == -2);
  static_assert(result[0][2] == -3);
}
}