#pragma once

/*
synopsis

namespace logging
{
template <class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
class basic_prefixed_syncstream : public std::basic_ostream<CharT, Traits>
{
  public:
    explicit basic_prefixed_syncstream(std::nullptr_t) noexcept;

    template <class Function>
    explicit basic_prefixed_syncstream(
      std::unique_ptr<basic_prefixed_syncbuf<Function, CharT, Traits, Allocator>> && buf) noexcept;
};

using prefixed_syncstream  = basic_prefixed_syncstream<char>;
using wprefixed_syncstream = basic_prefixed_syncstream<wchar_t>;
}
*/

#include <logging/prefixed_syncbuf.hpp>

#include <cstddef>
#include <memory>
#include <ostream>

namespace logging
{
template <class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
class basic_prefixed_syncstream : public std::basic_ostream<CharT, Traits>
{
  public:
    explicit basic_prefixed_syncstream(std::nullptr_t) noexcept
    : std::basic_ostream<CharT, Traits>(nullptr),
      buf(nullptr, [](void *) {})
    {
    }

    template <class Function>
    explicit basic_prefixed_syncstream(
      std::unique_ptr<basic_prefixed_syncbuf<Function, CharT, Traits, Allocator>> && buf) noexcept
    : std::basic_ostream<CharT, Traits>(buf.get()),
      buf(buf.release(),
          [](void * ptr)
          {
            std::default_delete<basic_prefixed_syncbuf<Function, CharT, Traits, Allocator>>{}(
              static_cast<basic_prefixed_syncbuf<Function, CharT, Traits, Allocator> *>(ptr));
          })
    {
    }

  private:
    std::unique_ptr<void, void (*)(void *)> buf;
};

using prefixed_syncstream  = basic_prefixed_syncstream<char>;
using wprefixed_syncstream = basic_prefixed_syncstream<wchar_t>;
}