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
    explicit basic_prefixedostream(std::nullptr_t) noexcept
    : std::basic_ostream<CharT, Traits>(nullptr),
      buf(nullptr, [](void *) {})
    {
    }

    template <class Function>
    explicit basic_prefixedostream(
      std::unique_ptr<basic_prefixedbuf<Function, CharT, Traits, Allocator>> && buf) noexcept
    : std::basic_ostream<CharT, Traits>(buf.get()),
      buf(buf.release(),
          [](void * ptr) { delete static_cast<basic_prefixedbuf<Function, CharT, Traits, Allocator> *>(ptr); })
    {
    }

  private:
    std::unique_ptr<void, void (*)(void *)> buf;
};

using prefixedostream  = basic_prefixedostream<char>;
using wprefixedostream = basic_prefixedostream<wchar_t>;
}