#pragma once

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
class basic_prefixedbuf : public std::basic_stringbuf<CharT, Traits, Allocator>
{
  public:
    explicit basic_prefixedbuf(std::basic_ostream<CharT, Traits> & wrapped, Function prefix)
    : wrapped(wrapped),
      prefix(std::move(prefix))
    {
    }

    basic_prefixedbuf(basic_prefixedbuf const &) = delete;

    basic_prefixedbuf(basic_prefixedbuf &&) = delete;

    basic_prefixedbuf & operator=(basic_prefixedbuf const &) = delete;

    basic_prefixedbuf & operator=(basic_prefixedbuf &&) = delete;

    ~basic_prefixedbuf()
    {
      try
      {
        basic_prefixedbuf::sync();
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
using prefixedbuf = basic_prefixedbuf<Function, char>;

template <class Function>
using wprefixedbuf = basic_prefixedbuf<Function, wchar_t>;
}