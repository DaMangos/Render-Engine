#include <maths/matrix.hpp>

#include <gtest/gtest.h>

namespace
{
TEST(ColumnViewsAssignment, RowMajorColumnViewsToRowMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::row_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = from.columns();

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 4);
  static_assert(to[2][0] == 7);

  static_assert(to[0][1] == 2);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 8);

  static_assert(to[0][2] == 3);
  static_assert(to[1][2] == 6);
  static_assert(to[2][2] == 9);
}

TEST(ColumnViewsAssignment, ColumnMajorColumnViewsToRowMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::row_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = from.columns();

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 4);
  static_assert(to[2][0] == 7);

  static_assert(to[0][1] == 2);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 8);

  static_assert(to[0][2] == 3);
  static_assert(to[1][2] == 6);
  static_assert(to[2][2] == 9);
}

TEST(ColumnViewsAssignment, RowMajorColumnViewsToColumnMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::column_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = from.columns();

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 4);
  static_assert(to[2][0] == 7);

  static_assert(to[0][1] == 2);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 8);

  static_assert(to[0][2] == 3);
  static_assert(to[1][2] == 6);
  static_assert(to[2][2] == 9);
}

TEST(ColumnViewsAssignment, ColumnMajorColumnViewsToColumnMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::column_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = from.columns();

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 4);
  static_assert(to[2][0] == 7);

  static_assert(to[0][1] == 2);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 8);

  static_assert(to[0][2] == 3);
  static_assert(to[1][2] == 6);
  static_assert(to[2][2] == 9);
}

TEST(ColumnViewsAssignment, RowMajorMatrixToRowMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::row_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = from;

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 4);
  static_assert(to[2][0] == 7);

  static_assert(to[0][1] == 2);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 8);

  static_assert(to[0][2] == 3);
  static_assert(to[1][2] == 6);
  static_assert(to[2][2] == 9);
}

TEST(ColumnViewsAssignment, ColumnMajorMatrixToRowMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::row_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = from;

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 4);
  static_assert(to[2][0] == 7);

  static_assert(to[0][1] == 2);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 8);

  static_assert(to[0][2] == 3);
  static_assert(to[1][2] == 6);
  static_assert(to[2][2] == 9);
}

TEST(ColumnViewsAssignment, RowMajorMatrixToColumnMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::column_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = from;

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 4);
  static_assert(to[2][0] == 7);

  static_assert(to[0][1] == 2);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 8);

  static_assert(to[0][2] == 3);
  static_assert(to[1][2] == 6);
  static_assert(to[2][2] == 9);
}

TEST(ColumnViewsAssignment, ColumnMajorMatrixToColumnMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> from;

    from[0][0] = 1, from[0][1] = 2, from[0][2] = 3;
    from[1][0] = 4, from[1][1] = 5, from[1][2] = 6;
    from[2][0] = 7, from[2][1] = 8, from[2][2] = 9;

    maths::matrix<int, 3, 3, maths::layout::column_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = from;

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 4);
  static_assert(to[2][0] == 7);

  static_assert(to[0][1] == 2);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 8);

  static_assert(to[0][2] == 3);
  static_assert(to[1][2] == 6);
  static_assert(to[2][2] == 9);
}

TEST(ColumnViewsAssignment, InitializerListToRowMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::row_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9},
    };

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 2);
  static_assert(to[2][0] == 3);

  static_assert(to[0][1] == 4);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 6);

  static_assert(to[0][2] == 7);
  static_assert(to[1][2] == 8);
  static_assert(to[2][2] == 9);
}

TEST(ColumnViewsAssignment, InitializerListToColumnMajorColumnViews)
{
  constexpr auto to = []()
  {
    maths::matrix<int, 3, 3, maths::layout::column_major> to;

    to[0][0] = 10, to[0][1] = 20, to[0][2] = 30;
    to[1][0] = 40, to[1][1] = 50, to[1][2] = 60;
    to[2][0] = 70, to[2][1] = 80, to[2][2] = 90;

    to.columns() = {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9},
    };

    return to;
  }();

  static_assert(to[0][0] == 1);
  static_assert(to[1][0] == 2);
  static_assert(to[2][0] == 3);

  static_assert(to[0][1] == 4);
  static_assert(to[1][1] == 5);
  static_assert(to[2][1] == 6);

  static_assert(to[0][2] == 7);
  static_assert(to[1][2] == 8);
  static_assert(to[2][2] == 9);
}
}