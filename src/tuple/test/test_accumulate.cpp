#include <tuple/accumulate.hpp>

#include <gtest/gtest.h>

#include <string>
#include <tuple>
#include <vector>

namespace
{
TEST(TupleAccumulate, AccumulatesValues)
{
  auto values = std::tuple{1, 2, 3, 4};

  EXPECT_EQ(tuple::accumulate(values, 0, std::plus<>{}), 10);
}

TEST(TupleAccumulate, DefaultOperation)
{
  auto values = std::tuple{1, 2, 3, 4};

  EXPECT_EQ(tuple::accumulate(values, 0), 10);
}

TEST(TupleAccumulate, UsesInitialValue)
{
  auto values = std::tuple{1, 2, 3};

  EXPECT_EQ(tuple::accumulate(values, 100), 106);
}

TEST(TupleAccumulate, ProcessesElementsInOrder)
{
  auto values = std::tuple{1, 2, 3};

  std::vector<int> order;

  auto result = tuple::accumulate(values,
                                  0,
                                  [&](int init, int value)
                                  {
                                    order.push_back(value);
                                    return init + value;
                                  });

  EXPECT_EQ(result, 6);
  EXPECT_EQ(order, (std::vector<int>{1, 2, 3}));
}

TEST(TupleAccumulate, SupportsCustomOperation)
{
  auto values = std::tuple{1, 2, 3, 4};

  EXPECT_EQ(tuple::accumulate(values, 1, [](int init, int value) { return init * value; }), 24);
}

TEST(TupleAccumulate, EmptyTuple)
{
  auto values = std::tuple{};

  EXPECT_EQ(tuple::accumulate(values, 42), 42);
}

TEST(TupleAccumulate, DifferentElementTypes)
{
  auto values = std::tuple{1, 2.5, 3};

  EXPECT_DOUBLE_EQ(tuple::accumulate(values, 0.0), 6.5);
}

TEST(TupleAccumulate, SupportsNonPrimitiveInitialValue)
{
  auto values = std::tuple{
    std::string{"a"},
    std::string{"b"},
    std::string{"c"},
  };

  auto result = tuple::accumulate(values,
                                  std::string{},
                                  [](std::string init, std::string const & value)
                                  {
                                    init += value;
                                    return init;
                                  });

  EXPECT_EQ(result, "abc");
}
}