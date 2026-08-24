#include <tuple/drop.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <string>

namespace
{
TEST(TupleDrop, DropsFirstNElements)
{
  auto tuple = std::tuple{1, 2.0, '3', 4u};

  auto result = tuple::drop<2>(tuple);

  EXPECT_EQ(std::get<0>(result), '3');
  EXPECT_EQ(std::get<1>(result), 4u);
}

TEST(TupleDrop, ZeroDropsNothing)
{
  auto tuple = std::tuple{1, 2.0, '3'};

  auto result = tuple::drop<0>(tuple);

  EXPECT_EQ(result, tuple);
}

TEST(TupleDrop, DropsEntireTuple)
{
  auto tuple = std::tuple{1, 2.0, '3'};

  auto result = tuple::drop<3>(tuple);

  static_assert(std::same_as<decltype(result), std::tuple<>>);

  EXPECT_TRUE(result == std::tuple{});
}

TEST(TupleDrop, LvalueReturnsReferences)
{
  auto tuple = std::tuple{1, 2.0, '3'};

  auto result = tuple::drop<1>(tuple);

  static_assert(std::same_as<decltype(result), std::tuple<double &, char &>>);

  std::get<0>(result) = 20.0;
  std::get<1>(result) = 'x';

  EXPECT_EQ(std::get<1>(tuple), 20.0);
  EXPECT_EQ(std::get<2>(tuple), 'x');
}

TEST(TupleDrop, ConstLvalueReturnsConstReferences)
{
  auto const tuple = std::tuple{1, 2.0, '3'};

  auto result = tuple::drop<1>(tuple);

  static_assert(std::same_as<decltype(result), std::tuple<double const &, char const &>>);

  EXPECT_EQ(std::get<0>(result), 2.0);
  EXPECT_EQ(std::get<1>(result), '3');
}

TEST(TupleDrop, RvalueReturnsValues)
{
  auto result = tuple::drop<1>(std::tuple{1, 2.0, '3'});

  static_assert(std::same_as<decltype(result), std::tuple<double, char>>);

  EXPECT_EQ(std::get<0>(result), 2.0);
  EXPECT_EQ(std::get<1>(result), '3');
}

TEST(TupleDrop, RvalueMovesElements)
{
  auto value = std::make_unique<std::string>("hello");

  auto source = std::tuple{42, std::move(value)};

  auto result = tuple::drop<1>(std::move(source));

  static_assert(std::same_as<decltype(result), std::tuple<std::unique_ptr<std::string>>>);

  EXPECT_NE(std::get<0>(result), nullptr);
  EXPECT_EQ(*std::get<0>(result), "hello");
}
}