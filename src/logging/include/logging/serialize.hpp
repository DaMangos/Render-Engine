#pragma once

#include <concepts>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace logging
{
template <class Type, class CharT, class Traits>
concept serializable_range = std::ranges::input_range<Type>
                         and not(std::convertible_to<std::decay_t<Type>, std::basic_string_view<CharT, Traits>>
                                 or std::convertible_to<std::decay_t<Type>, std::basic_string<CharT, Traits>>);

inline namespace serialize
{
template <class CharT, class Traits = std::char_traits<CharT>>
std::basic_ostream<CharT, Traits> & operator<<(std::basic_ostream<CharT, Traits> &       out,
                                               serializable_range<CharT, Traits> auto && range)
{
  out.put(CharT{'['});

  bool first = true;

  for(auto const & value : range)
  {
    if(first)
      first = false;
    else
      out.put(CharT{','}).put(CharT{' '});

    out << value;
  }

  out.put(CharT{']'});

  return out;
}

template <class CharT, class Traits, class... Types>
std::basic_ostream<CharT, Traits> & operator<<(std::basic_ostream<CharT, Traits> & out,
                                               std::tuple<Types...> const &        tuple)
{
  out.put(CharT{'{'});

  auto comma_fold = [&, first = true](auto const &... values) mutable
  {
    (((first ? (first = false, out) : out.put(CharT{','}).put(CharT{' '})) << values), ...);
  };

  std::apply(comma_fold, tuple);

  out.put(CharT{'}'});

  return out;
}

template <class CharT, class Traits = std::char_traits<CharT>, class First, class Second>
std::basic_ostream<CharT, Traits> & operator<<(std::basic_ostream<CharT, Traits> & out,
                                               std::pair<First, Second> const &    pair)
{
  out.put(CharT{'{'});

  out << pair.first;

  out.put(CharT{','}).put(CharT{' '});

  out << pair.second;

  out.put(CharT{'}'});

  return out;
}
}

template <class CharT = char, class Traits = std::char_traits<CharT>>
[[nodiscard]]
std::basic_string<CharT, Traits> to_string(auto && value)
{
  using namespace serialize;

  std::basic_stringstream<CharT, Traits> out;

  out << value;

  return out.str();
}
}