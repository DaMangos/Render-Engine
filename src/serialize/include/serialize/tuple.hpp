#pragma once

/*
synopsis


namespace serialize
{
inline namespace tuple
{
template <class Stream, class Tuple>
Stream && operator<<(Stream && out, Tuple && tuple)
  requires(tuple::tuple_like<Tuple>
           and not serializable_range<Tuple, Stream::char_type, Stream::char_type> ...);
}
}

*/

#include <serialize/serializable_range.hpp>
#include <tuple/tuple_like.hpp>

#include <concepts>
#include <tuple>
#include <type_traits>
#include <utility>

namespace serialize
{
inline namespace ranges
{
template <class Stream, class Range>
Stream && operator<<(Stream && out, Range && range)
  requires(std::derived_from<std::decay_t<Stream>, std::ios_base>
           and serializable_range<std::decay_t<Range>,
                                  typename std::decay_t<Stream>::char_type,
                                  typename std::decay_t<Stream>::traits_type>);
}

inline namespace tuple
{
template <class Stream, class Tuple>
Stream && operator<<(Stream && out, Tuple && tuple)
  requires(std::derived_from<std::decay_t<Stream>, std::ios_base> and ::tuple::tuple_like<std::decay_t<Tuple>>
           and not serializable_range<std::decay_t<Tuple>,
                                      typename std::decay_t<Stream>::char_type,
                                      typename std::decay_t<Stream>::traits_type>)
{
  using char_type   = typename std::remove_reference_t<Stream>::char_type;
  using traits_type = typename std::remove_reference_t<Stream>::traits_type;

  out.put(char_type{'{'});

  auto comma_fold = [&, first = true](auto const &... values) mutable
  {
    auto serialize = [&](auto && value)
    {
      if(!first)
        out << char_type{','} << char_type{' '};

      first = false;

      if constexpr(::tuple::tuple_like<std::decay_t<decltype(value)>>)
        (::serialize::tuple::operator<<)(out, std::forward<decltype(value)>(value));
      else if constexpr(serializable_range<std::decay_t<decltype(value)>, char_type, traits_type>)
        (::serialize::ranges::operator<<)(out, std::forward<decltype(value)>(value));
      else
        out << value;
    };

    (serialize(values), ...);
  };
  std::apply(comma_fold, tuple);

  out.put(char_type{'}'});

  return std::forward<Stream>(out);
}
}
}
