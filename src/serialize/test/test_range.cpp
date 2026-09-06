#include <serialize/ranges.hpp>
#include <serialize/tuple.hpp>

#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <forward_list>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace
{
using namespace serialize::ranges;
using namespace serialize::tuple;
using namespace std::literals;

TEST(SerializeRange, EmptyVector)
{
  std::ostringstream stream;

  stream << std::vector<int>{};

  EXPECT_EQ(stream.str(), "[]");
}

TEST(SerializeRange, Vector)
{
  std::ostringstream stream;

  stream << std::vector{1, 2, 3};

  EXPECT_EQ(stream.str(), "[1, 2, 3]");
}

TEST(SerializeRange, Array)
{
  std::ostringstream stream;

  stream << std::array{1, 2, 3};

  EXPECT_EQ(stream.str(), "[1, 2, 3]");
}

TEST(SerializeRange, List)
{
  std::ostringstream stream;

  stream << std::list{4, 5, 6};

  EXPECT_EQ(stream.str(), "[4, 5, 6]");
}

TEST(SerializeRange, RangeOfCharArrays)
{
  std::ostringstream stream;

  stream << std::vector{"one", "two", "three"};

  EXPECT_EQ(stream.str(), "[one, two, three]");
}

TEST(SerializeRange, RangeOfStrings)
{
  std::ostringstream stream;

  stream << std::vector{"one"s, "two"s, "three"s};

  EXPECT_EQ(stream.str(), "[one, two, three]");
}

TEST(SerializeRange, RangeOfStringViews)
{
  std::ostringstream stream;

  stream << std::vector{"one"sv, "two"sv, "three"sv};

  EXPECT_EQ(stream.str(), "[one, two, three]");
}

TEST(SerializeRange, RangeOfPaths)
{
  std::ostringstream stream;

  stream << std::vector<std::filesystem::path>{"one", "two", "three"};

  EXPECT_EQ(stream.str(), "[\"one\", \"two\", \"three\"]");
}

TEST(SerializeRange, StringAsCharArrays)
{
  std::ostringstream stream;

  stream << "hello";

  EXPECT_EQ(stream.str(), "hello");
}

TEST(SerializeRange, StringAsString)
{
  std::ostringstream stream;

  stream << "hello"s;

  EXPECT_EQ(stream.str(), "hello");
}

TEST(SerializeRange, StringViewAsString)
{
  std::ostringstream stream;

  stream << "hello"sv;

  EXPECT_EQ(stream.str(), "hello");
}

TEST(SerializeRange, StringViewAsPath)
{
  std::ostringstream stream;

  stream << std::filesystem::path{"hello"};

  EXPECT_EQ(stream.str(), "\"hello\"");
}

TEST(SerializeRange, NestedVectors)
{
  std::ostringstream stream;

  stream << std::vector{
    std::vector{1, 2},
    std::vector{3, 4},
    std::vector{5, 6}
  };

  EXPECT_EQ(stream.str(), "[[1, 2], [3, 4], [5, 6]]");
}

TEST(SerializeRange, EmptyNestedRanges)
{
  std::ostringstream stream;

  stream << std::vector{
    std::vector<int>{},
    std::vector{1, 2},
    std::vector<int>{}
  };

  EXPECT_EQ(stream.str(), "[[], [1, 2], []]");
}

TEST(SerializeRange, VectorOfTuples)
{
  std::ostringstream stream;

  stream << std::vector{
    std::tuple{1, 2},
    std::tuple{3, 4},
    std::tuple{5, 6}
  };

  EXPECT_EQ(stream.str(), "[{1, 2}, {3, 4}, {5, 6}]");
}

TEST(SerializeRange, ArrayOfTuples)
{
  std::ostringstream stream;

  stream << std::array{
    std::tuple{1, 2},
    std::tuple{3, 4}
  };

  EXPECT_EQ(stream.str(), "[{1, 2}, {3, 4}]");
}

TEST(SerializeRange, VectorOfCharacters)
{
  std::ostringstream stream;

  stream << std::vector{'a', 'b', 'c'};

  EXPECT_EQ(stream.str(), "[a, b, c]");
}

TEST(SerializeRange, WideCharacterRange)
{
  std::wostringstream stream;

  stream << std::vector{L'a', L'b', L'c'};

  EXPECT_EQ(stream.str(), L"[a, b, c]");
}

TEST(SerializeRange, WideString)
{
  std::wostringstream stream;

  stream << L"hello"s;

  EXPECT_EQ(stream.str(), L"hello");
}

TEST(SerializeRange, WithViews)
{
  std::ostringstream stream;

  stream << (std::forward_list{"foo"s, "bar"s, "baz"s} | std::views::transform([](auto const & str) { return str.front(); }));

  EXPECT_EQ(stream.str(), "[f, b, b]");
}

TEST(SerializeRange, ReturnsTheOriginalStream)
{
  std::ostringstream stream;

  auto & result = (stream << std::vector{1, 2, 3});

  EXPECT_EQ(&result, &stream);
}
}