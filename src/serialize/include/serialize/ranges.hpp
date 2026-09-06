#pragma once

/*
synopsis


namespace serialize
{
inline namespace ranges
{
template <class Stream, class Range>
Stream && operator<<(Stream && out, Range && range)
  requires(serializable_range<Tuple, Stream::char_type, Stream::char_type> ...);
}
}

*/

#include <serialize/serializable_range.hpp>
#include <tuple/tuple_like.hpp>

#include <concepts>
#include <type_traits>
#include <utility>

namespace serialize
{
inline namespace tuple
{
template <class Stream, class Tuple>
Stream && operator<<(Stream && out, Tuple && tuple)
  requires(std::derived_from<std::decay_t<Stream>, std::ios_base> and ::tuple::tuple_like<std::decay_t<Tuple>>
           and not serializable_range<std::decay_t<Tuple>,
                                      typename std::decay_t<Stream>::char_type,
                                      typename std::decay_t<Stream>::traits_type>);
}

inline namespace ranges
{
template <class Stream, class Range>
Stream && operator<<(Stream && out, Range && range) requires(std::derived_from<std::decay_t<Stream>, std::ios_base>
                                                             and serializable_range<std::decay_t<Range>,
                                                                                    typename std::decay_t<Stream>::char_type,
                                                                                    typename std::decay_t<Stream>::traits_type>)
{
  using char_type = typename std::remove_reference_t<Stream>::char_type;

  using traits_type = typename std::remove_reference_t<Stream>::traits_type;

  out.put(char_type{'['});

  bool first = true;

  for(auto && value : range)
  {
    if(first)
      first = false;
    else
      out.put(char_type{','}).put(char_type{' '});

    if constexpr(::tuple::tuple_like<std::decay_t<decltype(value)>>)
      (::serialize::tuple::operator<<)(out, std::forward<decltype(value)>(value));
    else if constexpr(serializable_range<std::decay_t<decltype(value)>, char_type, traits_type>)
      (::serialize::ranges::operator<<)(out, std::forward<decltype(value)>(value));
    else
      out << value;
  }

  out.put(char_type{']'});

  return std::forward<Stream>(out);
}
}
}
