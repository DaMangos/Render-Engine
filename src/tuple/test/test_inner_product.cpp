#include <tuple/inner_product.hpp>

#include <gtest/gtest.h>

#include <tuple>
#include <utility>
#include <vector>

namespace
{
TEST(TupleInnerProduct, CalculatesInnerProduct)
{
  auto lhs = std::tuple{1, 2, 3};
  auto rhs = std::tuple{4, 5, 6};

  EXPECT_EQ(tuple::inner_product(lhs, rhs, 0), 32);
}

TEST(TupleInnerProduct, UsesCustomOperations)
{
  auto lhs = std::tuple{1, 2, 3};
  auto rhs = std::tuple{4, 5, 6};

  auto result = tuple::inner_product(
    lhs,
    rhs,
    0,
    [](int init, int value) { return init + value; },
    [](int lhs_value, int rhs_value) { return lhs_value * rhs_value; });

  EXPECT_EQ(result, 32);
}

TEST(TupleInnerProduct, ProcessesElementsInOrder)
{
  auto lhs = std::tuple{1, 2, 3};
  auto rhs = std::tuple{4, 5, 6};

  std::vector<std::pair<int, int>> order;

  auto result = tuple::inner_product(
    lhs,
    rhs,
    0,
    [](int init, int value) { return init + value; },
    [&](int lhs_value, int rhs_value)
    {
      order.emplace_back(lhs_value, rhs_value);
      return lhs_value * rhs_value;
    });

  auto const old_order = std::vector{
    std::pair{1, 4},
    std::pair{2, 5},
    std::pair{3, 6},
  };

  EXPECT_EQ(result, 32);
  EXPECT_EQ(order, old_order);
}

TEST(TupleInnerProduct, UsesInitialValue)
{
  auto lhs = std::tuple{1, 2, 3};
  auto rhs = std::tuple{4, 5, 6};

  EXPECT_EQ(tuple::inner_product(lhs, rhs, 100), 132);
}

TEST(TupleInnerProduct, EmptyTuples)
{
  auto lhs = std::tuple{};
  auto rhs = std::tuple{};

  EXPECT_EQ(tuple::inner_product(lhs, rhs, 42), 42);
}

TEST(TupleInnerProduct, UsesShortestTuple)
{
  auto lhs = std::tuple{1, 2, 3};
  auto rhs = std::tuple{4, 5};

  EXPECT_EQ(tuple::inner_product(lhs, rhs, 0), 14);
}

TEST(TupleInnerProduct, DifferentElementTypes)
{
  auto lhs = std::tuple{1, 2, 3};
  auto rhs = std::tuple{0.5, 1.5, 2.5};

  EXPECT_DOUBLE_EQ(tuple::inner_product(lhs, rhs, 0.0), 11.0);
}
}