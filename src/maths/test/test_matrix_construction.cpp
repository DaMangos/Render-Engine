#include <maths/matrix.hpp>

#include <gtest/gtest.h>

#include <forward_list>
#include <list>
#include <mdspan>
#include <ranges>
#include <vector>

namespace
{
TEST(MatrixConstruction, DefaultConstructorOfRowMajorMatrix)
{
  constexpr maths::matrix<int, 2, 3, maths::layout::row_major> mat;

  static_assert(mat[0][0] == 0);
  static_assert(mat[0][1] == 0);
  static_assert(mat[0][2] == 0);
  static_assert(mat[1][0] == 0);
  static_assert(mat[1][1] == 0);
  static_assert(mat[1][2] == 0);

  static_assert(mat.data()[0] == 0);
  static_assert(mat.data()[1] == 0);
  static_assert(mat.data()[2] == 0);
  static_assert(mat.data()[3] == 0);
  static_assert(mat.data()[4] == 0);
  static_assert(mat.data()[5] == 0);
}

TEST(MatrixConstruction, DefaultConstructorOfColumnMajorMatrix)
{
  constexpr maths::matrix<int, 2, 3, maths::layout::column_major> mat;

  static_assert(mat[0][0] == 0);
  static_assert(mat[0][1] == 0);
  static_assert(mat[0][2] == 0);
  static_assert(mat[1][0] == 0);
  static_assert(mat[1][1] == 0);
  static_assert(mat[1][2] == 0);

  static_assert(mat.data()[0] == 0);
  static_assert(mat.data()[1] == 0);
  static_assert(mat.data()[2] == 0);
  static_assert(mat.data()[3] == 0);
  static_assert(mat.data()[4] == 0);
  static_assert(mat.data()[5] == 0);
}

TEST(MatrixConstruction, InitializerListToRowMajorMatrix)
{
  constexpr maths::matrix<int, 2, 3, maths::layout::row_major> mat = {1, 2, 3, 4, 5, 6};

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

TEST(MatrixConstruction, InitializerListToColumnMajorMatrix)
{
  constexpr maths::matrix<int, 2, 3, maths::layout::column_major> mat = {1, 2, 3, 4, 5, 6};

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

TEST(MatrixConstruction, InitializerListToRowMajorMatrixOverflow)
{
  constexpr maths::matrix<int, 2, 3, maths::layout::row_major> mat = {1, 2, 3, 4, 5, 6, 7, 8};

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

TEST(MatrixConstruction, InitializerListToColumnMajorMatrixOverflow)
{
  constexpr maths::matrix<int, 2, 3, maths::layout::column_major> mat = {1, 2, 3, 4, 5, 6, 7, 8};

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

TEST(MatrixConstruction, VectorToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    return maths::matrix<int, 2, 3, maths::layout::row_major>(std::from_range, std::vector{1, 2, 3, 4, 5, 6});
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

TEST(MatrixConstruction, VectorToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    return maths::matrix<int, 2, 3, maths::layout::column_major>(std::from_range, std::vector{1, 2, 3, 4, 5, 6});
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

TEST(MatrixConstruction, ListToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    return maths::matrix<int, 2, 3, maths::layout::row_major>(std::from_range, std::list{1, 2, 3, 4, 5, 6});
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

TEST(MatrixConstruction, ListToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    return maths::matrix<int, 2, 3, maths::layout::column_major>(std::from_range, std::list{1, 2, 3, 4, 5, 6});
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

TEST(MatrixConstruction, IterPairToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const list = std::forward_list{1, 2, 3, 4, 5, 6};

    return maths::matrix<int, 2, 3, maths::layout::row_major>(list.begin(), list.end());
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

TEST(MatrixConstruction, IterPairToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const list = std::forward_list{1, 2, 3, 4, 5, 6};

    return maths::matrix<int, 2, 3, maths::layout::column_major>(list.begin(), list.end());
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

TEST(MatrixConstruction, LayoutRightMdspanToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const vec = std::vector{1, 2, 3, 4, 5, 6};

    auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_right>{vec.data()};

    return maths::matrix<int, 2, 3, maths::layout::row_major>(md);
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

TEST(MatrixConstruction, LayoutLeftMdspanToRowMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const vec = std::vector{1, 2, 3, 4, 5, 6};

    auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_left>{vec.data()};

    return maths::matrix<int, 2, 3, maths::layout::row_major>(md);
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

TEST(MatrixConstruction, LayoutLeftMdspanToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const vec = std::vector{1, 2, 3, 4, 5, 6};

    auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_left>{vec.data()};

    return maths::matrix<int, 2, 3, maths::layout::column_major>(md);
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

TEST(MatrixConstruction, LayoutRightMdspanToColumnMajorMatrix)
{
  constexpr auto mat = []()
  {
    auto const vec = std::vector{1, 2, 3, 4, 5, 6};

    auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_right>{vec.data()};

    return maths::matrix<int, 2, 3, maths::layout::column_major>(md);
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
