#include <tuple/transfrom.hpp>

#include <gtest/gtest.h>

#include <tuple>

namespace
{
TEST(TupleTransfrom, IntToString)
{
  auto tuple = std::tuple{1, 2, 3};

  auto result = tuple::transfrom(tuple, [](int value) { return std::to_string(value); });

  static_assert(std::same_as<decltype(result), std::tuple<std::string, std::string, std::string>>);

  EXPECT_EQ(std::get<0>(result), "1");
  EXPECT_EQ(std::get<1>(result), "2");
  EXPECT_EQ(std::get<2>(result), "3");
}

TEST(TupleTransfrom, EmptyTuple)
{
  auto tuple = std::tuple{};

  int count = 0;

  auto result = tuple::transfrom(tuple,
                                 [&](auto)
                                 {
                                   ++count;
                                   return 1;
                                 });

  EXPECT_EQ(result, tuple);
  EXPECT_EQ(count, 0);
}
}