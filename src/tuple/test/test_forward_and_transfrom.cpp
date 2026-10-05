#include <tuple/forward_and_transfrom.hpp>

#include <gtest/gtest.h>

#include <tuple>

namespace
{
TEST(TupleForwardAndTransfrom, RvalueToLvalue)
{
  struct A
  {
      int i;
  };

  auto tuple = std::tuple{A{1}, A{2}, A{3}};

  auto result = tuple::forward_and_transfrom(tuple, [](A & a) -> auto & { return a.i; });

  static_assert(std::same_as<decltype(result), std::tuple<int &, int &, int &>>);

  EXPECT_EQ(&std::get<0>(result), &std::get<0>(tuple).i);
  EXPECT_EQ(&std::get<1>(result), &std::get<1>(tuple).i);
  EXPECT_EQ(&std::get<2>(result), &std::get<2>(tuple).i);
}

TEST(TupleForwardAndTransfrom, EmptyTuple)
{
  auto tuple = std::tuple{};

  int count = 0;

  auto result = tuple::forward_and_transfrom(tuple,
                                             [&](auto)
                                             {
                                               ++count;
                                               return 1;
                                             });

  EXPECT_EQ(result, tuple);
  EXPECT_EQ(count, 0);
}
}