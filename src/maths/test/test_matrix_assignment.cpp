#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <forward_list>
#include <list>
#include <mdspan>
#include <ranges>
#include <vector>

namespace
{
TEST(MatrixAssignment, InitializerListToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> mat;

    mat = {1, 2, 3, 4, 5, 6};

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 2);
  static_assert(mat.data()[2] == 3);
  static_assert(mat.data()[3] == 4);
  static_assert(mat.data()[4] == 5);
  static_assert(mat.data()[5] == 6);
}

TEST(MatrixAssignment, InitializerListToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> mat;

    mat = {1, 2, 3, 4, 5, 6};

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 4);
  static_assert(mat.data()[2] == 2);
  static_assert(mat.data()[3] == 5);
  static_assert(mat.data()[4] == 3);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, InitializerListToRowMajorMatrixOverflow)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> mat;

    mat = {1, 2, 3, 4, 5, 6, 7, 8};

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 2);
  static_assert(mat.data()[2] == 3);
  static_assert(mat.data()[3] == 4);
  static_assert(mat.data()[4] == 5);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, InitializerListToColumnMajorMatrixOverflow)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> mat;

    mat = {1, 2, 3, 4, 5, 6, 7, 8};

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 4);
  static_assert(mat.data()[2] == 2);
  static_assert(mat.data()[3] == 5);
  static_assert(mat.data()[4] == 3);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, VectorToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> mat;

    mat.assign(std::from_range, std::vector{1, 2, 3, 4, 5, 6});

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 2);
  static_assert(mat.data()[2] == 3);
  static_assert(mat.data()[3] == 4);
  static_assert(mat.data()[4] == 5);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, VectorToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> mat;

    mat.assign(std::from_range, std::vector{1, 2, 3, 4, 5, 6});

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 4);
  static_assert(mat.data()[2] == 2);
  static_assert(mat.data()[3] == 5);
  static_assert(mat.data()[4] == 3);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, ListToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::row_major> mat;

    mat.assign(std::from_range, std::list{1, 2, 3, 4, 5, 6});

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 2);
  static_assert(mat.data()[2] == 3);
  static_assert(mat.data()[3] == 4);
  static_assert(mat.data()[4] == 5);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, ListToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    maths::matrix<int, 2, 3, maths::layout::column_major> mat;

    mat.assign(std::from_range, std::list{1, 2, 3, 4, 5, 6});

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 4);
  static_assert(mat.data()[2] == 2);
  static_assert(mat.data()[3] == 5);
  static_assert(mat.data()[4] == 3);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, IterPairToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto list = std::forward_list{1, 2, 3, 4, 5, 6};

    maths::matrix<int, 2, 3, maths::layout::row_major> mat;

    mat.assign(list.begin(), list.end());

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 2);
  static_assert(mat.data()[2] == 3);
  static_assert(mat.data()[3] == 4);
  static_assert(mat.data()[4] == 5);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, IterPairToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto list = std::forward_list{1, 2, 3, 4, 5, 6};

    maths::matrix<int, 2, 3, maths::layout::column_major> mat;

    mat.assign(list.begin(), list.end());

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 4);
  static_assert(mat.data()[2] == 2);
  static_assert(mat.data()[3] == 5);
  static_assert(mat.data()[4] == 3);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, LayoutRightMdspanToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const vec = std::vector{1, 2, 3, 4, 5, 6};

    auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_right>{vec.data()};

    maths::matrix<int, 2, 3, maths::layout::row_major> mat;

    mat = md;

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 2);
  static_assert(mat.data()[2] == 3);
  static_assert(mat.data()[3] == 4);
  static_assert(mat.data()[4] == 5);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, LayoutLeftMdspanToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const vec = std::vector{1, 2, 3, 4, 5, 6};

    auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_left>{vec.data()};

    maths::matrix<int, 2, 3, maths::layout::row_major> mat;

    mat = md;

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 3);
  static_assert(mat[0][2] == 5);
  static_assert(mat[1][0] == 2);
  static_assert(mat[1][1] == 4);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 3);
  static_assert(mat.data()[2] == 5);
  static_assert(mat.data()[3] == 2);
  static_assert(mat.data()[4] == 4);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, LayoutLeftMdspanToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const vec = std::vector{1, 2, 3, 4, 5, 6};

    auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_left>{vec.data()};

    maths::matrix<int, 2, 3, maths::layout::column_major> mat;

    mat = md;

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 3);
  static_assert(mat[0][2] == 5);
  static_assert(mat[1][0] == 2);
  static_assert(mat[1][1] == 4);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 2);
  static_assert(mat.data()[2] == 3);
  static_assert(mat.data()[3] == 4);
  static_assert(mat.data()[4] == 5);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}

TEST(MatrixAssignment, LayoutRightMdspanToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const vec = std::vector{1, 2, 3, 4, 5, 6};

    auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_right>{vec.data()};

    maths::matrix<int, 2, 3, maths::layout::column_major> mat;

    mat = md;

    return mat;
  }();

  static_assert(mat[0][0] == 1);
  static_assert(mat[0][1] == 2);
  static_assert(mat[0][2] == 3);
  static_assert(mat[1][0] == 4);
  static_assert(mat[1][1] == 5);
  static_assert(mat[1][2] == 6);

  static_assert(mat.data()[0] == 1);
  static_assert(mat.data()[1] == 4);
  static_assert(mat.data()[2] == 2);
  static_assert(mat.data()[3] == 5);
  static_assert(mat.data()[4] == 3);
  static_assert(mat.data()[5] == 6);

  static_assert(mat.size() == 6uz);
}
}
