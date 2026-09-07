#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <concepts>
#include <utility>

namespace
{
TEST(MatrixMultiplicationAssignment, RowMajorMatrixTimesRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;

    maths::matrix<int, 2, 2, maths::layout::row_major> y;

    y[0][0] = 5, y[0][1] = 6;
    y[1][0] = 7, y[1][1] = 8;

    auto & result = (x *= y);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::row_major> &>);

    return x;
  }();

  static_assert(x[0][0] == 19);
  static_assert(x[0][1] == 22);

  static_assert(x[1][0] == 43);
  static_assert(x[1][1] == 50);
}

TEST(MatrixMultiplicationAssignment, ColumnMajorMatrixTimesRowMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;

    maths::matrix<int, 2, 2, maths::layout::row_major> y;

    y[0][0] = 5, y[0][1] = 6;
    y[1][0] = 7, y[1][1] = 8;

    auto & result = (x *= y);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::column_major> &>);

    return x;
  }();

  static_assert(x[0][0] == 19);
  static_assert(x[0][1] == 22);

  static_assert(x[1][0] == 43);
  static_assert(x[1][1] == 50);
}

TEST(MatrixMultiplicationAssignment, RowMajorMatrixTimesColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;

    maths::matrix<int, 2, 2, maths::layout::column_major> y;

    y[0][0] = 5, y[0][1] = 6;
    y[1][0] = 7, y[1][1] = 8;

    auto & result = (x *= y);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::row_major> &>);

    return x;
  }();

  static_assert(x[0][0] == 19);
  static_assert(x[0][1] == 22);

  static_assert(x[1][0] == 43);
  static_assert(x[1][1] == 50);
}

TEST(MatrixMultiplicationAssignment, ColumnMajorMatrixTimesColumnMajorMatrix)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;

    maths::matrix<int, 2, 2, maths::layout::column_major> y;

    y[0][0] = 5, y[0][1] = 6;
    y[1][0] = 7, y[1][1] = 8;

    auto & result = (x *= y);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::column_major> &>);

    return x;
  }();

  static_assert(x[0][0] == 19);
  static_assert(x[0][1] == 22);

  static_assert(x[1][0] == 43);
  static_assert(x[1][1] == 50);
}

TEST(MatrixMultiplicationAssignment, RowMajorMatrixTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    auto & result = (x *= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 3, maths::layout::row_major> &>);

    return x;
  }();

  static_assert(x[0][0] == 2);
  static_assert(x[0][1] == 4);
  static_assert(x[0][2] == 6);

  static_assert(x[1][0] == 8);
  static_assert(x[1][1] == 10);
  static_assert(x[1][2] == 12);
}

TEST(MatrixMultiplicationAssignment, ColumnMajorMatrixTimesScalar)
{
  constexpr auto x = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    auto & result = (x *= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 3, maths::layout::column_major> &>);

    return x;
  }();

  static_assert(x[0][0] == 2);
  static_assert(x[0][1] == 4);
  static_assert(x[0][2] == 6);

  static_assert(x[1][0] == 8);
  static_assert(x[1][1] == 10);
  static_assert(x[1][2] == 12);
}

TEST(MatrixMultiplicationAssignment, RvalueRowMajorMatrixTimesRowMajorMatrix)
{
  constexpr auto result = []()
  {
    maths::matrix<int, 2, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;

    maths::matrix<int, 2, 2, maths::layout::row_major> y;

    y[0][0] = 5, y[0][1] = 6;
    y[1][0] = 7, y[1][1] = 8;

    auto result = (std::move(x) *= y);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::row_major>>);

    return result;
  }();

  static_assert(result[0][0] == 19);
  static_assert(result[0][1] == 22);

  static_assert(result[1][0] == 43);
  static_assert(result[1][1] == 50);
}

TEST(MatrixMultiplicationAssignment, RvalueColumnMajorMatrixTimesRowMajorMatrix)
{
  constexpr auto result = []()
  {
    maths::matrix<int, 2, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;

    maths::matrix<int, 2, 2, maths::layout::row_major> y;

    y[0][0] = 5, y[0][1] = 6;
    y[1][0] = 7, y[1][1] = 8;

    auto result = (std::move(x) *= y);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::column_major>>);

    return result;
  }();

  static_assert(result[0][0] == 19);
  static_assert(result[0][1] == 22);

  static_assert(result[1][0] == 43);
  static_assert(result[1][1] == 50);
}

TEST(MatrixMultiplicationAssignment, RvalueRowMajorMatrixTimesColumnMajorMatrix)
{
  constexpr auto result = []()
  {
    maths::matrix<int, 2, 2, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;

    maths::matrix<int, 2, 2, maths::layout::column_major> y;

    y[0][0] = 5, y[0][1] = 6;
    y[1][0] = 7, y[1][1] = 8;

    auto result = (std::move(x) *= y);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::row_major>>);

    return result;
  }();

  static_assert(result[0][0] == 19);
  static_assert(result[0][1] == 22);

  static_assert(result[1][0] == 43);
  static_assert(result[1][1] == 50);
}

TEST(MatrixMultiplicationAssignment, RvalueColumnMajorMatrixTimesColumnMajorMatrix)
{
  constexpr auto result = []()
  {
    maths::matrix<int, 2, 2, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2;
    x[1][0] = 3, x[1][1] = 4;

    maths::matrix<int, 2, 2, maths::layout::column_major> y;

    y[0][0] = 5, y[0][1] = 6;
    y[1][0] = 7, y[1][1] = 8;

    auto result = (std::move(x) *= y);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 2, maths::layout::column_major>>);

    return result;
  }();

  static_assert(result[0][0] == 19);
  static_assert(result[0][1] == 22);

  static_assert(result[1][0] == 43);
  static_assert(result[1][1] == 50);
}

TEST(MatrixMultiplicationAssignment, RvalueRowMajorMatrixTimesScalar)
{
  constexpr auto result = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    auto result = (std::move(x) *= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 3, maths::layout::row_major>>);

    return result;
  }();

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);

  static_assert(result[1][0] == 8);
  static_assert(result[1][1] == 10);
  static_assert(result[1][2] == 12);
}

TEST(MatrixMultiplicationAssignment, RvalueColumnMajorMatrixTimesScalar)
{
  constexpr auto result = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> x;

    x[0][0] = 1, x[0][1] = 2, x[0][2] = 3;
    x[1][0] = 4, x[1][1] = 5, x[1][2] = 6;

    auto result = (std::move(x) *= 2);

    static_assert(std::same_as<decltype(result), maths::matrix<int, 2, 3, maths::layout::column_major>>);

    return result;
  }();

  static_assert(result[0][0] == 2);
  static_assert(result[0][1] == 4);
  static_assert(result[0][2] == 6);

  static_assert(result[1][0] == 8);
  static_assert(result[1][1] == 10);
  static_assert(result[1][2] == 12);
}
}
