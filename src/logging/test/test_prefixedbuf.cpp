#include <logging/prefixedbuf.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace
{
TEST(Prefixedbuf, AddsPrefix)
{
  std::ostringstream output;

  logging::basic_prefixedbuf buffer{output, [] { return std::string{"[TEST] "}; }};

  std::ostream stream{&buffer};

  stream << "hello" << std::flush;

  EXPECT_EQ(output.str(), "[TEST] hello\n");
}

TEST(Prefixedbuf, PrefixIsGeneratedWhenFlushed)
{
  std::ostringstream output;

  int prefix_count = 0;

  logging::basic_prefixedbuf buffer{output,
                                    [&]
                                    {
                                      ++prefix_count;
                                      return std::string{"prefix "};
                                    }};

  std::ostream stream{&buffer};

  EXPECT_EQ(prefix_count, 0);

  stream << "one" << std::flush;

  EXPECT_EQ(prefix_count, 1);

  stream << "two" << std::flush;

  EXPECT_EQ(prefix_count, 2);

  EXPECT_EQ(output.str(), "prefix one\nprefix two\n");
}

TEST(Prefixedbuf, MultipleWritesAreCombined)
{
  std::ostringstream output;

  logging::basic_prefixedbuf buffer{output, [] { return std::string{"[TEST] "}; }};

  std::ostream stream{&buffer};

  stream << "hello ";
  stream << "world";
  stream.flush();

  EXPECT_EQ(output.str(), "[TEST] hello world\n");
}

TEST(Prefixedbuf, EmptyFlushDoesNothing)
{
  std::ostringstream output;

  logging::basic_prefixedbuf buffer{output, [] { return std::string{"[TEST] "}; }};

  std::ostream stream{&buffer};

  stream.flush();

  EXPECT_TRUE(output.str().empty());
}

TEST(Prefixedbuf, BufferIsClearedAfterFlush)
{
  std::ostringstream output;

  logging::basic_prefixedbuf buffer{output, [] { return std::string{"[TEST] "}; }};

  std::ostream stream{&buffer};

  EXPECT_TRUE(buffer.str().empty());

  stream << "hello";

  EXPECT_FALSE(buffer.str().empty());

  stream << std::flush;

  EXPECT_TRUE(buffer.str().empty());
}

TEST(Prefixedbuf, PrefixCanDependOnState)
{
  std::ostringstream output;

  int value = 0;

  logging::basic_prefixedbuf buffer{output, [&] { return std::to_string(value) + ": "; }};

  std::ostream stream{&buffer};

  value = 1;
  stream << "one" << std::flush;

  value = 2;
  stream << "two" << std::flush;

  EXPECT_EQ(output.str(),
            "1: one\n"
            "2: two\n");
}

TEST(Prefixedbuf, DestructorFlushesBuffer)
{
  std::ostringstream output;

  {
    logging::basic_prefixedbuf buffer{output, [] { return std::string{"[TEST] "}; }};

    std::ostream stream{&buffer};

    stream << "hello";
  }

  EXPECT_EQ(output.str(), "[TEST] hello\n");
}

TEST(Prefixedbuf, FailingWrappedStreamCausesFlushToFail)
{
  std::ostringstream output;

  logging::basic_prefixedbuf buffer{output, [] { return std::string{"[TEST] "}; }};

  std::ostream stream{&buffer};

  output.setstate(std::ios::failbit);

  stream << "hello";

  EXPECT_TRUE(stream.flush().fail());
  EXPECT_TRUE(output.str().empty());
}

TEST(Prefixedbuf, DifferentBuffersCanWrapDifferentStreams)
{
  std::ostringstream output1;
  std::ostringstream output2;

  logging::basic_prefixedbuf buffer1{output1, [] { return std::string{"one: "}; }};
  logging::basic_prefixedbuf buffer2{output2, [] { return std::string{"two: "}; }};

  std::ostream stream1{&buffer1};
  std::ostream stream2{&buffer2};

  stream1 << "hello" << std::flush;
  stream2 << "world" << std::flush;

  EXPECT_EQ(output1.str(), "one: hello\n");
  EXPECT_EQ(output2.str(), "two: world\n");
}

TEST(Prefixedbuf, WideCharacters)
{
  std::wostringstream output;

  logging::wprefixedbuf<decltype([] { return std::wstring{L"[TEST] "}; })> buffer{output, {}};

  std::wostream stream{&buffer};

  stream << L"hello" << std::flush;

  EXPECT_EQ(output.str(), L"[TEST] hello\n");
}

TEST(Prefixedbuf, WideCharactersMultipleWrites)
{
  std::wostringstream output;

  logging::wprefixedbuf<decltype([] { return std::wstring{L"[TEST] "}; })> buffer{output, {}};

  std::wostream stream{&buffer};

  stream << L"hello ";
  stream << L"world";
  stream.flush();

  EXPECT_EQ(output.str(), L"[TEST] hello world\n");
}

TEST(Prefixedbuf, ConcurrentWritesAreNotInterleaved)
{
  std::stringstream inoutput;

  constexpr int thread_count        = 50;
  constexpr int messages_per_thread = 100;

  std::vector<std::jthread> threads;

  for(int thread = 0; thread < thread_count; ++thread)
  {
    threads.emplace_back(
      [&inoutput, thread]
      {
        logging::basic_prefixedbuf buffer{inoutput, [] { return std::string{"[TEST] "}; }};

        std::ostream stream{&buffer};

        for(int message = 0; message < messages_per_thread; ++message)
          stream << thread << ':' << message << std::flush;
      });
  }

  for(auto & thread : threads)
    thread.join();

  std::string line;

  int line_count = 0;

  while(std::getline(inoutput, line))
  {
    EXPECT_TRUE(line.starts_with("[TEST] "));
    ++line_count;
  }

  EXPECT_EQ(line_count, thread_count * messages_per_thread);
}
}