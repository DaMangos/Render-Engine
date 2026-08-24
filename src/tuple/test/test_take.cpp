#include <tuple/take.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <tuple>
#include <utility>

namespace
{
TEST(TupleTake, TakesFirstNElements)
{
  auto tuple = std::tuple{1, 2.0, '3', 4u};

  auto result = tuple::take<2>(tuple);

  EXPECT_EQ(std::get<0>(result), 1);
  EXPECT_EQ(std::get<1>(result), 2.0);
}

TEST(TupleTake, ZeroTakesEmptyTuple)
{
  auto tuple = std::tuple{1, 2.0, '3'};

  auto result = tuple::take<0>(tuple);

  static_assert(std::same_as<decltype(result), std::tuple<>>);
  EXPECT_TRUE(result == std::tuple{});
}

TEST(TupleTake, TakesEntireTuple)
{
  auto tuple = std::tuple{1, 2.0, '3'};

  auto result = tuple::take<3>(tuple);

  EXPECT_EQ(result, tuple);
}

TEST(TupleTake, LvalueReturnsReferences)
{
  auto tuple = std::tuple{1, 2.0, '3'};

  auto result = tuple::take<2>(tuple);

  static_assert(std::same_as<decltype(result), std::tuple<int &, double &>>);

  std::get<0>(result) = 10;
  std::get<1>(result) = 20.0;

  EXPECT_EQ(std::get<0>(tuple), 10);
  EXPECT_EQ(std::get<1>(tuple), 20.0);
}

TEST(TupleTake, ConstLvalueReturnsConstReferences)
{
  auto const tuple = std::tuple{1, 2.0, '3'};

  auto result = tuple::take<2>(tuple);

  static_assert(std::same_as<decltype(result), std::tuple<int const &, double const &>>);

  EXPECT_EQ(std::get<0>(result), 1);
  EXPECT_EQ(std::get<1>(result), 2.0);
}

TEST(TupleTake, RvalueReturnsValues)
{
  auto result = tuple::take<2>(std::tuple{1, 2.0, '3'});

  static_assert(std::same_as<decltype(result), std::tuple<int, double>>);

  EXPECT_EQ(std::get<0>(result), 1);
  EXPECT_EQ(std::get<1>(result), 2.0);
}

TEST(TupleTake, RvalueMovesElements)
{
  auto value = std::make_unique<std::string>("hello");

  auto source = std::tuple{std::move(value), 42};

  auto result = tuple::take<1>(std::move(source));

  static_assert(std::same_as<decltype(result), std::tuple<std::unique_ptr<std::string>>>);

  EXPECT_NE(std::get<0>(result), nullptr);
  EXPECT_EQ(*std::get<0>(result), "hello");
}
}