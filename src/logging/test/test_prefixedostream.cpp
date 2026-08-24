#include <logging/prefixed_syncbuf.hpp>
#include <logging/prefixed_syncstream.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>

namespace
{
TEST(prefixed_syncstream, NullStreamIsConstructible)
{
  logging::prefixed_syncstream stream{nullptr};

  EXPECT_TRUE(stream.bad());

  stream << "hello";

  EXPECT_TRUE(stream.bad());
}

TEST(prefixed_syncstream, WritesToWrappedStream)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixed_syncbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixed_syncstream stream{std::move(buffer)};

  stream << "hello" << std::flush;

  EXPECT_EQ(output.str(), "[TEST] hello\n");
}

TEST(prefixed_syncstream, MultipleWritesAreCombined)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixed_syncbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixed_syncstream stream{std::move(buffer)};

  stream << "hello ";
  stream << "world";

  stream << std::flush;

  EXPECT_EQ(output.str(), "[TEST] hello world\n");
}

TEST(prefixed_syncstream, MultipleMessagesGetSeparatePrefixes)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixed_syncbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixed_syncstream stream{std::move(buffer)};

  stream << "first" << std::flush;
  stream << "second" << std::flush;

  EXPECT_EQ(output.str(),
            "[TEST] first\n"
            "[TEST] second\n");
}

TEST(prefixed_syncstream, PrefixIsEvaluatedForEachMessage)
{
  std::ostringstream output;

  int count = 0;

  auto prefix = [&]
  {
    return std::to_string(++count) + ": ";
  };

  auto buffer = std::make_unique<logging::basic_prefixed_syncbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixed_syncstream stream{std::move(buffer)};

  stream << "first" << std::flush;
  stream << "second" << std::flush;
  stream << "third" << std::flush;

  EXPECT_EQ(count, 3);

  EXPECT_EQ(output.str(),
            "1: first\n"
            "2: second\n"
            "3: third\n");
}

TEST(prefixed_syncstream, FlushesThroughOstream)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixed_syncbuf<decltype(prefix), char>>(output, std::move(prefix));

  logging::prefixed_syncstream stream{std::move(buffer)};

  stream << "hello";

  EXPECT_TRUE(output.str().empty());

  stream.flush();

  EXPECT_EQ(output.str(), "[TEST] hello\n");
}

TEST(prefixed_syncstream, MovedUniquePtrIsEmpty)
{
  std::ostringstream output;

  auto prefix = []
  {
    return std::string{"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixed_syncbuf<decltype(prefix), char>>(output, std::move(prefix));

  auto * buffer_ptr = buffer.get();

  logging::prefixed_syncstream stream{std::move(buffer)};

  EXPECT_EQ(buffer, nullptr);

  stream << "hello" << std::flush;

  EXPECT_EQ(output.str(), "[TEST] hello\n");
  EXPECT_EQ(stream.rdbuf(), buffer_ptr);
}

TEST(prefixed_syncstream, WideCharacters)
{
  std::wostringstream output;

  auto prefix = []
  {
    return std::wstring{L"[INFO] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixed_syncbuf<decltype(prefix), wchar_t>>(output, std::move(prefix));

  logging::wprefixed_syncstream stream{std::move(buffer)};

  stream << L"hello" << std::flush;

  EXPECT_EQ(output.str(), L"[INFO] hello\n");
}

TEST(prefixed_syncstream, WideCharactersMultipleWrites)
{
  std::wostringstream output;

  auto prefix = []
  {
    return std::wstring{L"[TEST] "};
  };

  auto buffer = std::make_unique<logging::basic_prefixed_syncbuf<decltype(prefix), wchar_t>>(output, std::move(prefix));

  logging::wprefixed_syncstream stream{std::move(buffer)};

  stream << L"hello ";
  stream << L"world";
  stream.flush();

  EXPECT_EQ(output.str(), L"[TEST] hello world\n");
}
}