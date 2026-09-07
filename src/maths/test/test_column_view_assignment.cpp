#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(ColumnViewAssignment, RowMajorColumnViewToRowMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 4, from[0][2] = 7;
    from[1][0] = 2, from[1][1] = 5, from[1][2] = 8;
    from[2][0] = 3, from[2][1] = 6, from[2][2] = 9;

    maths::matrix<int, 3, 2, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = from.column(0);

    return to;
  }();

  static_assert(to[0][0] == to[0][1]);
  static_assert(to[1][0] == to[1][1]);
  static_assert(to[2][0] == to[2][1]);
}

TEST(ColumnViewAssignment, ColumnMajorColumnViewToRowMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 4, from[0][2] = 7;
    from[1][0] = 2, from[1][1] = 5, from[1][2] = 8;
    from[2][0] = 3, from[2][1] = 6, from[2][2] = 9;

    maths::matrix<int, 3, 2, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = from.column(0);

    return to;
  }();

  static_assert(to[0][0] == to[0][1]);
  static_assert(to[1][0] == to[1][1]);
  static_assert(to[2][0] == to[2][1]);
}

TEST(ColumnViewAssignment, RowMajorColumnViewToColumnMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 4, from[0][2] = 7;
    from[1][0] = 2, from[1][1] = 5, from[1][2] = 8;
    from[2][0] = 3, from[2][1] = 6, from[2][2] = 9;

    maths::matrix<int, 3, 2, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = from.column(0);

    return to;
  }();

  static_assert(to[0][0] == to[0][1]);
  static_assert(to[1][0] == to[1][1]);
  static_assert(to[2][0] == to[2][1]);
}

TEST(ColumnViewAssignment, ColumnMajorColumnViewToColumnMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 4, from[0][2] = 7;
    from[1][0] = 2, from[1][1] = 5, from[1][2] = 8;
    from[2][0] = 3, from[2][1] = 6, from[2][2] = 9;

    maths::matrix<int, 3, 2, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = from.column(0);

    return to;
  }();

  static_assert(to[0][0] == to[0][1]);
  static_assert(to[1][0] == to[1][1]);
  static_assert(to[2][0] == to[2][1]);
}

TEST(ColumnViewAssignment, RowMajorMatrixToRowMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 1, maths::layout::row_major> from;

    from[0][0] = 1;
    from[1][0] = 2;
    from[2][0] = 3;

    maths::matrix<int, 3, 2, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = from;

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 2);
  static_assert(to[2][0] == 3);

  static_assert(to[0][1] == 1);
  static_assert(to[1][1] == 2);
  static_assert(to[2][1] == 3);
}

TEST(ColumnViewAssignment, ColumnMajorMatrixToRowMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 1, maths::layout::column_major> from;

    from[0][0] = 1;
    from[1][0] = 2;
    from[2][0] = 3;

    maths::matrix<int, 3, 2, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = from;

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 2);
  static_assert(to[2][0] == 3);

  static_assert(to[0][1] == 1);
  static_assert(to[1][1] == 2);
  static_assert(to[2][1] == 3);
}

TEST(ColumnViewAssignment, RowMajorMatrixToColumnMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 1, maths::layout::row_major> from;

    from[0][0] = 1;
    from[1][0] = 2;
    from[2][0] = 3;

    maths::matrix<int, 3, 2, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = from;

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 2);
  static_assert(to[2][0] == 3);

  static_assert(to[0][1] == 1);
  static_assert(to[1][1] == 2);
  static_assert(to[2][1] == 3);
}

TEST(ColumnViewAssignment, ColumnMajorMatrixToColumnMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 1, maths::layout::column_major> from;

    from[0][0] = 1;
    from[1][0] = 2;
    from[2][0] = 3;

    maths::matrix<int, 3, 2, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = from;

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 2);
  static_assert(to[2][0] == 3);

  static_assert(to[0][1] == 1);
  static_assert(to[1][1] == 2);
  static_assert(to[2][1] == 3);
}

TEST(ColumnViewAssignment, InitializerListToRowMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 2, maths::layout::row_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = {1, 2, 3};

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 2);
  static_assert(to[2][0] == 3);

  static_assert(to[0][1] == 1);
  static_assert(to[1][1] == 2);
  static_assert(to[2][1] == 3);
}

TEST(ColumnViewAssignment, InitializerListToColumnMajorColumnView)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 2, maths::layout::column_major> to;

    to[0][0] = 1, to[0][1] = 4;
    to[1][0] = 2, to[1][1] = 5;
    to[2][0] = 3, to[2][1] = 6;

    to.column(1) = {1, 2, 3};

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 2);
  static_assert(to[2][0] == 3);

  static_assert(to[0][1] == 1);
  static_assert(to[1][1] == 2);
  static_assert(to[2][1] == 3);
}
}