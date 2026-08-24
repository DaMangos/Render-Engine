#include <logging/serialize.hpp>

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
using namespace logging::serialize;
using namespace std::literals;

TEST(Serialize, SerializesEmptyVector)
{
  std::ostringstream stream;

  stream << std::vector<int>{};

  EXPECT_EQ(stream.str(), "[]");
}

TEST(Serialize, SerializesVector)
{
  std::ostringstream stream;

  stream << std::vector{1, 2, 3};

  EXPECT_EQ(stream.str(), "[1, 2, 3]");
}

TEST(Serialize, SerializesArray)
{
  std::ostringstream stream;

  stream << std::array{1, 2, 3};

  EXPECT_EQ(stream.str(), "[1, 2, 3]");
}

TEST(Serialize, SerializesList)
{
  std::ostringstream stream;

  stream << std::list{4, 5, 6};

  EXPECT_EQ(stream.str(), "[4, 5, 6]");
}

TEST(Serialize, SerializesRangeOfCharArrays)
{
  std::ostringstream stream;

  stream << std::vector{"one", "two", "three"};

  EXPECT_EQ(stream.str(), "[one, two, three]");
}

TEST(Serialize, SerializesRangeOfStrings)
{
  std::ostringstream stream;

  stream << std::vector{"one"s, "two"s, "three"s};

  EXPECT_EQ(stream.str(), "[one, two, three]");
}

TEST(Serialize, SerializesRangeOfStringViews)
{
  std::ostringstream stream;

  stream << std::vector{"one"sv, "two"sv, "three"sv};

  EXPECT_EQ(stream.str(), "[one, two, three]");
}

TEST(Serialize, SerializesRangeOfPaths)
{
  std::ostringstream stream;

  stream << std::vector<std::filesystem::path>{"one", "two", "three"};

  EXPECT_EQ(stream.str(), "[\"one\", \"two\", \"three\"]");
}

TEST(Serialize, SerializesStringAsCharArrays)
{
  std::ostringstream stream;

  stream << "hello";

  EXPECT_EQ(stream.str(), "hello");
}

TEST(Serialize, SerializesStringAsString)
{
  std::ostringstream stream;

  stream << "hello"s;

  EXPECT_EQ(stream.str(), "hello");
}

TEST(Serialize, SerializesStringViewAsString)
{
  std::ostringstream stream;

  stream << "hello"sv;

  EXPECT_EQ(stream.str(), "hello");
}

TEST(Serialize, SerializesStringViewAsPath)
{
  std::ostringstream stream;

  stream << std::filesystem::path{"hello"};

  EXPECT_EQ(stream.str(), "\"hello\"");
}

TEST(Serialize, SerializesNestedVectors)
{
  std::ostringstream stream;

  stream << std::vector{
    std::vector{1, 2},
    std::vector{3, 4},
    std::vector{5, 6}
  };

  EXPECT_EQ(stream.str(), "[[1, 2], [3, 4], [5, 6]]");
}

TEST(Serialize, SerializesEmptyNestedRanges)
{
  std::ostringstream stream;

  stream << std::vector{
    std::vector<int>{},
    std::vector{1, 2},
    std::vector<int>{}
  };

  EXPECT_EQ(stream.str(), "[[], [1, 2], []]");
}

TEST(Serialize, SerializesTuple)
{
  std::ostringstream stream;

  stream << std::tuple{1, 2, 3};

  EXPECT_EQ(stream.str(), "{1, 2, 3}");
}

TEST(Serialize, SerializesEmptyTuple)
{
  std::ostringstream stream;

  stream << std::tuple{};

  EXPECT_EQ(stream.str(), "{}");
}

TEST(Serialize, SerializesTupleWithDifferentTypes)
{
  std::ostringstream stream;

  stream << std::tuple{42, "hello"s, 3.14};

  EXPECT_EQ(stream.str(), "{42, hello, 3.14}");
}

TEST(Serialize, SerializesNestedTuple)
{
  std::ostringstream stream;

  stream << std::tuple{
    1,
    std::tuple{2, 3},
    4
  };

  EXPECT_EQ(stream.str(), "{1, {2, 3}, 4}");
}

TEST(Serialize, SerializesVectorOfTuples)
{
  std::ostringstream stream;

  stream << std::vector{
    std::tuple{1, 2},
    std::tuple{3, 4},
    std::tuple{5, 6}
  };

  EXPECT_EQ(stream.str(), "[{1, 2}, {3, 4}, {5, 6}]");
}

TEST(Serialize, SerializesArrayOfTuples)
{
  std::ostringstream stream;

  stream << std::array{
    std::tuple{1, 2},
    std::tuple{3, 4}
  };

  EXPECT_EQ(stream.str(), "[{1, 2}, {3, 4}]");
}

TEST(Serialize, SerializesTupleContainingVector)
{
  std::ostringstream stream;

  stream << std::tuple{
    1,
    std::vector{2, 3},
    4
  };

  EXPECT_EQ(stream.str(), "{1, [2, 3], 4}");
}

TEST(Serialize, SerializesNestedTupleAndRange)
{
  std::ostringstream stream;

  stream << std::tuple{
    std::vector{                1, 2},
    std::tuple{std::vector{3, 4}, 5}
  };

  EXPECT_EQ(stream.str(), "{[1, 2], {[3, 4], 5}}");
}

TEST(Serialize, SerializesVectorOfCharacters)
{
  std::ostringstream stream;

  stream << std::vector{'a', 'b', 'c'};

  EXPECT_EQ(stream.str(), "[a, b, c]");
}

TEST(Serialize, SerializesWideCharacterRange)
{
  std::wostringstream stream;

  stream << std::vector{L'a', L'b', L'c'};

  EXPECT_EQ(stream.str(), L"[a, b, c]");
}

TEST(Serialize, SerializesWideString)
{
  std::wostringstream stream;

  stream << L"hello"s;

  EXPECT_EQ(stream.str(), L"hello");
}

TEST(Serialize, SerializesWithViews)
{
  std::ostringstream stream;

  stream << (std::forward_list{"foo"s, "bar"s, "baz"s}
             | std::views::transform([](auto const & str) { return str.front(); }));

  EXPECT_EQ(stream.str(), "[f, b, b]");
}

TEST(Serialize, ReturnsTheOriginalStream)
{
  std::ostringstream stream;

  auto & result = (stream << std::vector{1, 2, 3});

  EXPECT_EQ(&result, &stream);
}

TEST(Serialize, ReturnsTheOriginalStreamForTuple)
{
  std::ostringstream stream;

  auto & result = (stream << std::tuple{1, 2, 3});

  EXPECT_EQ(&result, &stream);
}
}