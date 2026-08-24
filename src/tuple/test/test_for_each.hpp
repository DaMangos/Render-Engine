#include <tuple/for_each.hpp>

#include <gtest/gtest.h>

#include <tuple>
#include <utility>

namespace
{
TEST(TupleForEach, MutableLvalue)
{
  auto tuple = std::tuple{1, 2, 3};

  tuple::for_each(tuple, [](auto & value) { value *= 2; });

  EXPECT_EQ(std::get<0>(tuple), 2);
  EXPECT_EQ(std::get<1>(tuple), 4);
  EXPECT_EQ(std::get<2>(tuple), 6);
}

TEST(TupleForEach, ConstLvalue)
{
  auto const tuple = std::tuple{1, 2, 3};

  int sum = 0;

  tuple::for_each(tuple, [&](auto const & value) { sum += value; });

  EXPECT_EQ(sum, 6);
}

TEST(TupleForEach, rvalue)
{
  auto tuple = std::tuple{std::make_unique<int>(1)};

  tuple::for_each(std::move(tuple),
                  [](std::unique_ptr<int> && value)
                  {
                    EXPECT_NE(value, nullptr);
                    EXPECT_EQ(*value, 1);
                  });
}

TEST(TupleForEach, EmptyTuple)
{
  auto tuple = std::tuple{};

  int count = 0;

  tuple::for_each(tuple, [&] { ++count; });

  EXPECT_EQ(count, 0);
}
}