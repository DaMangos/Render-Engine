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
  maths::matrix<int, 2, 3, maths::layout::row_major> mat;

  EXPECT_EQ(mat[0][0], 0);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[0][2], 0);
  EXPECT_EQ(mat[1][0], 0);
  EXPECT_EQ(mat[1][1], 0);
  EXPECT_EQ(mat[1][2], 0);

  EXPECT_EQ(mat.data()[0], 0);
  EXPECT_EQ(mat.data()[1], 0);
  EXPECT_EQ(mat.data()[2], 0);
  EXPECT_EQ(mat.data()[3], 0);
  EXPECT_EQ(mat.data()[4], 0);
  EXPECT_EQ(mat.data()[5], 0);
}

TEST(MatrixConstruction, DefaultConstructorOfColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> mat;

  EXPECT_EQ(mat[0][0], 0);
  EXPECT_EQ(mat[0][1], 0);
  EXPECT_EQ(mat[0][2], 0);
  EXPECT_EQ(mat[1][0], 0);
  EXPECT_EQ(mat[1][1], 0);
  EXPECT_EQ(mat[1][2], 0);

  EXPECT_EQ(mat.data()[0], 0);
  EXPECT_EQ(mat.data()[1], 0);
  EXPECT_EQ(mat.data()[2], 0);
  EXPECT_EQ(mat.data()[3], 0);
  EXPECT_EQ(mat.data()[4], 0);
  EXPECT_EQ(mat.data()[5], 0);
}

TEST(MatrixConstruction, InitializerListToRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> mat = {1, 2, 3, 4, 5, 6};

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 2);
  EXPECT_EQ(mat.data()[2], 3);
  EXPECT_EQ(mat.data()[3], 4);
  EXPECT_EQ(mat.data()[4], 5);
  EXPECT_EQ(mat.data()[5], 6);
}

TEST(MatrixConstruction, InitializerListToColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> mat = {1, 2, 3, 4, 5, 6};

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 4);
  EXPECT_EQ(mat.data()[2], 2);
  EXPECT_EQ(mat.data()[3], 5);
  EXPECT_EQ(mat.data()[4], 3);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, InitializerListToRowMajorMatrixOverflow)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> mat = {1, 2, 3, 4, 5, 6, 7, 8};

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 2);
  EXPECT_EQ(mat.data()[2], 3);
  EXPECT_EQ(mat.data()[3], 4);
  EXPECT_EQ(mat.data()[4], 5);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, InitializerListToColumnMajorMatrixOverflow)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> mat = {1, 2, 3, 4, 5, 6, 7, 8};

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 4);
  EXPECT_EQ(mat.data()[2], 2);
  EXPECT_EQ(mat.data()[3], 5);
  EXPECT_EQ(mat.data()[4], 3);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, VectorToRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> mat(std::from_range, std::vector{1, 2, 3, 4, 5, 6});

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 2);
  EXPECT_EQ(mat.data()[2], 3);
  EXPECT_EQ(mat.data()[3], 4);
  EXPECT_EQ(mat.data()[4], 5);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, VectorToColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> mat(std::from_range, std::vector{1, 2, 3, 4, 5, 6});

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 4);
  EXPECT_EQ(mat.data()[2], 2);
  EXPECT_EQ(mat.data()[3], 5);
  EXPECT_EQ(mat.data()[4], 3);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, ListToRowMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::row_major> mat(std::from_range, std::list{1, 2, 3, 4, 5, 6});

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 2);
  EXPECT_EQ(mat.data()[2], 3);
  EXPECT_EQ(mat.data()[3], 4);
  EXPECT_EQ(mat.data()[4], 5);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, ListToColumnMajorMatrix)
{
  maths::matrix<int, 2, 3, maths::layout::column_major> mat(std::from_range, std::list{1, 2, 3, 4, 5, 6});

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 4);
  EXPECT_EQ(mat.data()[2], 2);
  EXPECT_EQ(mat.data()[3], 5);
  EXPECT_EQ(mat.data()[4], 3);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, IterPairToRowMajorMatrix)
{
  auto const list = std::forward_list{1, 2, 3, 4, 5, 6};

  maths::matrix<int, 2, 3, maths::layout::row_major> mat(list.begin(), list.end());

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 2);
  EXPECT_EQ(mat.data()[2], 3);
  EXPECT_EQ(mat.data()[3], 4);
  EXPECT_EQ(mat.data()[4], 5);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, IterPairToColumnMajorMatrix)
{
  auto const list = std::forward_list{1, 2, 3, 4, 5, 6};

  maths::matrix<int, 2, 3, maths::layout::column_major> mat(list.begin(), list.end());

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 4);
  EXPECT_EQ(mat.data()[2], 2);
  EXPECT_EQ(mat.data()[3], 5);
  EXPECT_EQ(mat.data()[4], 3);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, LayoutRightMdspanToRowMajorMatrix)
{
  auto const vec = std::vector{1, 2, 3, 4, 5, 6};

  auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_right>{vec.data()};

  maths::matrix<int, 2, 3, maths::layout::row_major> mat(md);

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 2);
  EXPECT_EQ(mat.data()[2], 3);
  EXPECT_EQ(mat.data()[3], 4);
  EXPECT_EQ(mat.data()[4], 5);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat[0][0], (md[0, 0]));
  EXPECT_EQ(mat[0][1], (md[0, 1]));
  EXPECT_EQ(mat[0][2], (md[0, 2]));
  EXPECT_EQ(mat[1][0], (md[1, 0]));
  EXPECT_EQ(mat[1][1], (md[1, 1]));
  EXPECT_EQ(mat[1][2], (md[1, 2]));

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, LayoutLeftMdspanToRowMajorMatrix)
{
  auto const vec = std::vector{1, 2, 3, 4, 5, 6};

  auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_left>{vec.data()};

  maths::matrix<int, 2, 3, maths::layout::row_major> mat(md);

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 3);
  EXPECT_EQ(mat[0][2], 5);
  EXPECT_EQ(mat[1][0], 2);
  EXPECT_EQ(mat[1][1], 4);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 3);
  EXPECT_EQ(mat.data()[2], 5);
  EXPECT_EQ(mat.data()[3], 2);
  EXPECT_EQ(mat.data()[4], 4);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat[0][0], (md[0, 0]));
  EXPECT_EQ(mat[0][1], (md[0, 1]));
  EXPECT_EQ(mat[0][2], (md[0, 2]));
  EXPECT_EQ(mat[1][0], (md[1, 0]));
  EXPECT_EQ(mat[1][1], (md[1, 1]));
  EXPECT_EQ(mat[1][2], (md[1, 2]));

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, LayoutLeftMdspanToColumnMajorMatrix)
{
  auto const vec = std::vector{1, 2, 3, 4, 5, 6};

  auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_left>{vec.data()};

  maths::matrix<int, 2, 3, maths::layout::column_major> mat(md);

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 3);
  EXPECT_EQ(mat[0][2], 5);
  EXPECT_EQ(mat[1][0], 2);
  EXPECT_EQ(mat[1][1], 4);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 2);
  EXPECT_EQ(mat.data()[2], 3);
  EXPECT_EQ(mat.data()[3], 4);
  EXPECT_EQ(mat.data()[4], 5);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat[0][0], (md[0, 0]));
  EXPECT_EQ(mat[0][1], (md[0, 1]));
  EXPECT_EQ(mat[0][2], (md[0, 2]));
  EXPECT_EQ(mat[1][0], (md[1, 0]));
  EXPECT_EQ(mat[1][1], (md[1, 1]));
  EXPECT_EQ(mat[1][2], (md[1, 2]));

  EXPECT_EQ(mat.size(), 6uz);
}

TEST(MatrixConstruction, LayoutRightMdspanToColumnMajorMatrix)
{
  auto const vec = std::vector{1, 2, 3, 4, 5, 6};

  auto const md = std::mdspan<int const, std::extents<std::size_t, 2, 3>, std::layout_right>{vec.data()};

  maths::matrix<int, 2, 3, maths::layout::column_major> mat(md);

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][1], 2);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][0], 4);
  EXPECT_EQ(mat[1][1], 5);
  EXPECT_EQ(mat[1][2], 6);

  EXPECT_EQ(mat.data()[0], 1);
  EXPECT_EQ(mat.data()[1], 4);
  EXPECT_EQ(mat.data()[2], 2);
  EXPECT_EQ(mat.data()[3], 5);
  EXPECT_EQ(mat.data()[4], 3);
  EXPECT_EQ(mat.data()[5], 6);

  EXPECT_EQ(mat[0][0], (md[0, 0]));
  EXPECT_EQ(mat[0][1], (md[0, 1]));
  EXPECT_EQ(mat[0][2], (md[0, 2]));
  EXPECT_EQ(mat[1][0], (md[1, 0]));
  EXPECT_EQ(mat[1][1], (md[1, 1]));
  EXPECT_EQ(mat[1][2], (md[1, 2]));

  EXPECT_EQ(mat.size(), 6uz);
}
}
