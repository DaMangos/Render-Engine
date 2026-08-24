#pragma once

#include <concepts>
#include <tuple>
#include <type_traits>

namespace tuple
{
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
}