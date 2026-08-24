#pragma once

/*
synopsis

namespace logging
{
template <class Function, class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
class basic_prefixed_syncbuf : public std::basic_stringbuf<CharT, Traits, Allocator>
{
  public:
    explicit basic_prefixed_syncbuf(std::basic_ostream<CharT, Traits> & wrapped, Function prefix);

    basic_prefixed_syncbuf(basic_prefixed_syncbuf const &) = delete;

    basic_prefixed_syncbuf(basic_prefixed_syncbuf &&) = delete;

    basic_prefixed_syncbuf & operator=(basic_prefixed_syncbuf const &) = delete;

    basic_prefixed_syncbuf & operator=(basic_prefixed_syncbuf &&) = delete;

    ~basic_prefixed_syncbuf();

  protected:
    int sync() override;
};

template <class Function>
using prefixed_syncbuf = basic_prefixed_syncbuf<Function, char>;

template <class Function>
using wprefixed_syncbuf = basic_prefixed_syncbuf<Function, wchar_t>;
}
*/

#include <array>
#include <functional>
#include <memory>
#include <mutex>
#include <ostream>
#include <sstream>

namespace logging
{
namespace detail
{
[[nodiscard]]
inline constexpr std::mutex & get_mutex(void const * const buf) noexcept
{
  static std::array<std::mutex, 31> mutexes;
  return mutexes[std::hash<void const *>{}(buf) % mutexes.size()];
}
}

template <class Function, class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
class basic_prefixed_syncbuf : public std::basic_stringbuf<CharT, Traits, Allocator>
{
  public:
    explicit basic_prefixed_syncbuf(std::basic_ostream<CharT, Traits> & wrapped, Function prefix)
    : wrapped(wrapped),
      prefix(std::move(prefix))
    {
    }

    basic_prefixed_syncbuf(basic_prefixed_syncbuf const &) = delete;

    basic_prefixed_syncbuf(basic_prefixed_syncbuf &&) = delete;

    basic_prefixed_syncbuf & operator=(basic_prefixed_syncbuf const &) = delete;

    basic_prefixed_syncbuf & operator=(basic_prefixed_syncbuf &&) = delete;

    ~basic_prefixed_syncbuf()
    {
      try
      {
        basic_prefixed_syncbuf::sync();
      }
      catch(...)
      {
      }
    }

  protected:
    int sync() override
    {
      if(wrapped.get().fail())
        return -1;

      if(this->view().empty())
        return 0;

      auto const msg = prefix();

      {
        auto const lock = std::lock_guard(detail::get_mutex(wrapped.get().rdbuf()));

        wrapped.get() << msg << this->view() << std::endl;
      }

      if(wrapped.get().fail())
        return -1;

      this->str({});

      return 0;
    }

  private:
    std::reference_wrapper<std::basic_ostream<CharT, Traits>> wrapped;

    [[no_unique_address]]
    Function prefix;
};

template <class Function>
using prefixed_syncbuf = basic_prefixed_syncbuf<Function, char>;

template <class Function>
using wprefixed_syncbuf = basic_prefixed_syncbuf<Function, wchar_t>;
}