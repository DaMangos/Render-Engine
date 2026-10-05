#pragma once

#include <tuple/tuple_like.hpp>

#include <tuple>
#include <type_traits>

namespace tuple
{
template <class Tuple, class UnaryFunc>
requires(tuple_like<std::decay_t<Tuple>>)
[[nodiscard]]
constexpr auto transfrom(Tuple && tuple, UnaryFunc func)
{
  return [&]<std::size_t... I>(std::index_sequence<I...>)
  {
    return std::make_tuple(func(std::get<I>(std::forward<Tuple>(tuple)))...);
  }(std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>());
}

template <class Tuple, class UnaryFunc>
requires(tuple_like<std::decay_t<Tuple>>)
[[nodiscard]]
constexpr auto forward_and_transfrom(Tuple && tuple, UnaryFunc func)
{
  return [&]<std::size_t... I>(std::index_sequence<I...>)
  {
    return std::forward_as_tuple(func(std::get<I>(std::forward<Tuple>(tuple)))...);
  }(std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>());
}
}
