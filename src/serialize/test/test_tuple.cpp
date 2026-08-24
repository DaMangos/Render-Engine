#include <serialize/ranges.hpp>
#include <serialize/tuple.hpp>

#include <gtest/gtest.h>

#include <array>
#include <sstream>
#include <tuple>
#include <vector>

namespace
{
using namespace serialize::ranges;
using namespace serialize::tuple;

TEST(SerializeTuple, Empty)
{
  std::ostringstream stream;

  stream << std::tuple{};

  EXPECT_EQ(stream.str(), "{}");
}

TEST(SerializeTuple, SameTypes)
{
  std::ostringstream stream;

  stream << std::tuple{1, 2, 3};

  EXPECT_EQ(stream.str(), "{1, 2, 3}");
}

TEST(SerializeTuple, DifferentTypes)
{
  std::ostringstream stream;

  stream << std::tuple{42, "hello", 3.14};

  EXPECT_EQ(stream.str(), "{42, hello, 3.14}");
}

TEST(SerializeTuple, NestedTuple)
{
  std::ostringstream stream;

  stream << std::tuple{
    1,
    std::tuple{2, 3},
    4
  };

  EXPECT_EQ(stream.str(), "{1, {2, 3}, 4}");
}

TEST(SerializeTuple, VectorOfTuples)
{
  std::ostringstream stream;

  stream << std::vector{
    std::tuple{1, 2},
    std::tuple{3, 4},
    std::tuple{5, 6}
  };

  EXPECT_EQ(stream.str(), "[{1, 2}, {3, 4}, {5, 6}]");
}

TEST(SerializeTuple, ArrayOfTuples)
{
  std::ostringstream stream;

  stream << std::array{
    std::tuple{1, 2},
    std::tuple{3, 4}
  };

  EXPECT_EQ(stream.str(), "[{1, 2}, {3, 4}]");
}

TEST(SerializeTuple, TupleContainingVector)
{
  std::ostringstream stream;

  stream << std::tuple{
    1,
    std::vector{2, 3},
    4
  };

  EXPECT_EQ(stream.str(), "{1, [2, 3], 4}");
}

TEST(SerializeTuple, NestedTupleAndRange)
{
  std::ostringstream stream;

  stream << std::tuple{
    std::tuple{std::vector{3, 4}, 5}
  };

  EXPECT_EQ(stream.str(), "{[3, 4], 5}");
}

TEST(SerializeTuple, ReturnsTheOriginalStreamForTuple)
{
  std::ostringstream stream;

  auto & result = (stream << std::tuple{1, 2, 3});

  EXPECT_EQ(&result, &stream);
}
}