#include <logging/prefixedbuf.hpp>
#include <logging/prefixedostream.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>

namespace
{
TEST(Prefixedostream, NullStreamIsConstructible)
{
  logging::prefixedostream stream{nullptr};

  EXPECT_TRUE(stream.bad());

  stream << "hello";

  EXPECT_TRUE(stream.bad());
}

TEST(Prefixedostream, WritesToWrappedStream)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixedbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixedostream stream{std::move(buffer)};

  stream << "hello" << std::flush;

  EXPECT_EQ(output.str(), "[TEST] hello\n");
}

TEST(Prefixedostream, MultipleWritesAreCombined)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixedbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixedostream stream{std::move(buffer)};

  stream << "hello ";
  stream << "world";

  stream << std::flush;

  EXPECT_EQ(output.str(), "[TEST] hello world\n");
}

TEST(Prefixedostream, MultipleMessagesGetSeparatePrefixes)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixedbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixedostream stream{std::move(buffer)};

  stream << "first" << std::flush;
  stream << "second" << std::flush;

  EXPECT_EQ(output.str(),
            "[TEST] first\n"
            "[TEST] second\n");
}

TEST(Prefixedostream, PrefixIsEvaluatedForEachMessage)
{
  std::ostringstream output;

  int count = 0;

  auto prefix = [&]
  {
    return std::to_string(++count) + ": ";
  };

  auto buffer = std::make_unique<logging::basic_prefixedbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixedostream stream{std::move(buffer)};

  stream << "first" << std::flush;
  stream << "second" << std::flush;
  stream << "third" << std::flush;

  EXPECT_EQ(count, 3);

  EXPECT_EQ(output.str(),
            "1: first\n"
            "2: second\n"
            "3: third\n");
}

TEST(Prefixedostream, FlushesThroughOstream)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixedbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixedostream stream{std::move(buffer)};

  stream << "hello";

  EXPECT_TRUE(output.str().empty());

  stream.flush();

  EXPECT_EQ(output.str(), "[TEST] hello\n");
}

TEST(Prefixedostream, MovedUniquePtrIsEmpty)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixedbuf<decltype(prefix), char>>(output, std::move(prefix));

  auto * buffer_ptr = buffer.get();

  logging::prefixedostream stream{std::move(buffer)};

  EXPECT_EQ(buffer, nullptr);

  stream << "hello" << std::flush;

  EXPECT_EQ(output.str(), "[TEST] hello\n");
  EXPECT_EQ(stream.rdbuf(), buffer_ptr);
}

TEST(Prefixedostream, WideCharacters)
{
  std::wostringstream output;

  auto prefix = []
  {
    return std::wstring{L"[INFO] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixedbuf<decltype(prefix), wchar_t>>(output, std::move(prefix));

  logging::wprefixedostream stream{std::move(buffer)};

  stream << L"hello" << std::flush;

  EXPECT_EQ(output.str(), L"[INFO] hello\n");
}

TEST(Prefixedostream, WideCharactersMultipleWrites)
{
  std::wostringstream output;

  auto prefix = []
  {
    return std::wstring{L"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixedbuf<decltype(prefix), wchar_t>>(output, std::move(prefix));

  logging::wprefixedostream stream{std::move(buffer)};

  stream << L"hello ";
  stream << L"world";
  stream.flush();

  EXPECT_EQ(output.str(), L"[TEST] hello world\n");
}
}