#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(RowViewAssignment, RowMajorRowViewToRowMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = from.row(0);

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}

TEST(RowViewAssignment, ColumnMajorRowViewToRowMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = from.row(0);

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}

TEST(RowViewAssignment, RowMajorRowViewToColumnMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = from.row(0);

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}

TEST(RowViewAssignment, ColumnMajorRowViewToColumnMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 2, 3, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = from.row(0);

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}

TEST(RowViewAssignment, RowMajorMatrixToRowMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 1, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;

    maths::matrix<int, 2, 3, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = from;

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}

TEST(RowViewAssignment, ColumnMajorMatrixToRowMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;

    maths::matrix<int, 2, 3, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = from;

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}

TEST(RowViewAssignment, RowMajorMatrixToColumnMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 1, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;

    maths::matrix<int, 2, 3, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = from;

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}

TEST(RowViewAssignment, ColumnMajorMatrixToColumnMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 1, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;

    maths::matrix<int, 2, 3, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = from;

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}

TEST(RowViewAssignment, InitializerListToRowMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = {1, 2, 3};

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}

TEST(RowViewAssignment, InitializerListToColumnMajorRowView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 2, to[0][2] = 3;
    to[1][0] = 4, to[1][1] = 5, to[1][2] = 6;

    to.row(1) = {1, 2, 3};

    return to;
  }();

  static_assert(to[0][0] == to[1][0]);
  static_assert(to[0][1] == to[1][1]);
  static_assert(to[0][2] == to[1][2]);
}
}