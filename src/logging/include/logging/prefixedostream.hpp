#pragma once

#include <logging/prefixedbuf.hpp>

#include <cstddef>
#include <memory>
#include <ostream>

namespace logging
{
template <class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
class basic_prefixedostream : public std::basic_ostream<CharT, Traits>
{
  public:
    explicit basic_prefixedostream(std::nullptr_t)
    : std::basic_ostream<CharT, Traits>(nullptr),
      buf(nullptr)
    {
    }

    template <class Function>
    explicit basic_prefixedostream(std::unique_ptr<basic_prefixedbuf<Function, CharT, Traits, Allocator>> && buf)
    : std::basic_ostream<CharT, Traits>(buf.get()),
      buf(std::move(buf))
    {
    }

    basic_prefixedostream(basic_prefixedostream const &) = delete;

    basic_prefixedostream(basic_prefixedostream &&) = delete;

    basic_prefixedostream & operator=(basic_prefixedostream const &) = delete;

    basic_prefixedostream & operator=(basic_prefixedostream &&) = delete;

    ~basic_prefixedostream() = default;

  private:
    std::shared_ptr<void> buf;
};

using prefixedostream  = basic_prefixedostream<char>;
using wprefixedostream = basic_prefixedostream<wchar_t>;
}