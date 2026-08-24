#pragma once

#include <concepts>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace logging
{
template <class Range, class CharT, class Traits>
concept serializable_range = std::ranges::input_range<std::decay_t<Range>>
                         and not(std::convertible_to<std::decay_t<Range>, std::basic_string_view<CharT, Traits>>
                                 or std::convertible_to<std::decay_t<Range>, std::basic_string<CharT, Traits>>);

template <class Tuple, std::size_t N>
concept has_tuple_element = requires(Tuple t) {
  typename std::tuple_element_t<N, std::remove_const_t<Tuple>>;
  { get<N>(t) } -> std::convertible_to<std::tuple_element_t<N, Tuple> const &>;
};

template <class Tuple>
concept tuple_like = not std::is_reference_v<Tuple> and requires(Tuple t) {
  typename std::tuple_size<Tuple>::type;
  requires std::derived_from<std::tuple_size<Tuple>, std::integral_constant<std::size_t, std::tuple_size_v<Tuple>>>;
} and[]<std::size_t... N>(std::index_sequence<N...>)
{
  return (has_tuple_element<Tuple, N> and ...);
}

(std::make_index_sequence<std::tuple_size_v<Tuple>>());

inline namespace serialize
{

template <class Stream, class Tuple>
Stream && operator<<(Stream && out, Tuple && tuple)
  requires(std::derived_from<std::decay_t<Stream>, std::ios_base> and tuple_like<std::decay_t<Tuple>>
           and not serializable_range<std::decay_t<Tuple>,
                                      typename std::decay_t<Stream>::char_type,
                                      typename std::decay_t<Stream>::traits_type>);

template <class Stream, class Range>
Stream && operator<<(Stream && out, Range && range)
  requires(std::derived_from<std::decay_t<Stream>, std::ios_base>
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

    if constexpr(tuple_like<std::decay_t<decltype(value)>>
                 or serializable_range<std::decay_t<decltype(value)>, char_type, traits_type>)
      (logging::serialize::operator<<)(out, std::forward<decltype(value)>(value));
    else
      out << value;
  }

  out.put(char_type{']'});

  return std::forward<Stream>(out);
}

template <class Stream, class Tuple>
Stream && operator<<(Stream && out, Tuple && tuple)
  requires(std::derived_from<std::decay_t<Stream>, std::ios_base> and tuple_like<std::decay_t<Tuple>>
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

      if constexpr(tuple_like<std::decay_t<decltype(value)>>
                   or serializable_range<std::decay_t<decltype(value)>, char_type, traits_type>)
        (logging::serialize::operator<<)(out, std::forward<decltype(value)>(value));
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