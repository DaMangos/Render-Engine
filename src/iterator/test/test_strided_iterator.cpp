#include <iterator/strided_iterator.hpp>

#include <gtest/gtest.h>

#include <array>
#include <iterator>

namespace
{
TEST(StridedIterator, Increment)
{
  std::array values = {1, 2, 3, 4, 5, 6};

  iterator::strided_iterator<std::array<int, 6>::iterator, 2>       first{values.begin()};
  iterator::strided_iterator<std::array<int, 6>::iterator, 2> const last{values.end()};

  EXPECT_EQ(*first, 1);

  first++;

  EXPECT_EQ(*first, 3);

  first++;

  EXPECT_EQ(*first, 5);

  first++;

  EXPECT_EQ(last, first);
}

TEST(StridedIterator, Dincrement)
{
  std::array values = {1, 2, 3, 4, 5, 6};

  iterator::strided_iterator<std::array<int, 6>::iterator, 2> const first{values.begin()};
  iterator::strided_iterator<std::array<int, 6>::iterator, 2>       last{values.end()};

  last--;

  EXPECT_EQ(*last, 5);

  last--;

  EXPECT_EQ(*last, 3);

  last--;

  EXPECT_EQ(*last, 1);

  EXPECT_EQ(last, first);
}

TEST(StridedIterator, OperatorSubscript)
{
  std::array values = {1, 2, 3, 4, 5, 6};

  iterator::strided_iterator<std::array<int, 6>::iterator, 2> first{values.begin()};

  EXPECT_EQ(first[0], 1);
  EXPECT_EQ(first[1], 3);
  EXPECT_EQ(first[2], 5);
}

TEST(StridedIterator, Distance)
{
  std::array values = {1, 2, 3, 4, 5, 6};

  iterator::strided_iterator<std::array<int, 6>::iterator, 2> first{values.begin()};
  iterator::strided_iterator<std::array<int, 6>::iterator, 2> last{values.end()};

  EXPECT_EQ(std::ranges::distance(first, last), 3);
  EXPECT_EQ(last - first, 3);
}

TEST(StridedIterator, ConstIteratorConversion)
{
  std::array values = {1, 2, 3};

  iterator::strided_iterator<int *, 1>       iter{values.data()};
  iterator::strided_iterator<int const *, 1> const_iter{iter};

  EXPECT_EQ(const_iter.base(), iter.base());
}
}
