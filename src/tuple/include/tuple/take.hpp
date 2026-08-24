#pragma once

#include <tuple/tuple_like.hpp>

#include <tuple>
#include <type_traits>

namespace tuple
{
template <std::size_t N, class Tuple>
requires(tuple_like<std::decay_t<Tuple>> and N <= std::tuple_size_v<std::decay_t<Tuple>>)
[[nodiscard]]
constexpr auto take(Tuple & tuple) noexcept
{
  return [&]<std::size_t... I>(std::index_sequence<I...>)
  {
    return std::forward_as_tuple(std::get<I>(tuple)...);
  }(std::make_index_sequence<N>());
}

template <std::size_t N, class Tuple>
requires(tuple_like<std::decay_t<Tuple>> and N <= std::tuple_size_v<std::decay_t<Tuple>>)
[[nodiscard]]
constexpr auto take(Tuple && tuple)
{
  return [&]<std::size_t... I>(std::index_sequence<I...>)
  {
    return std::make_tuple(std::get<I>(std::move(tuple))...);
  }(std::make_index_sequence<N>());
}

template <std::size_t N, class Tuple>
requires(tuple_like<std::decay_t<Tuple>> and N <= std::tuple_size_v<std::decay_t<Tuple>>)
[[nodiscard]]
constexpr auto take(Tuple const & tuple) noexcept
{
  return [&]<std::size_t... I>(std::index_sequence<I...>)
  {
    return std::forward_as_tuple(std::get<I>(tuple)...);
  }(std::make_index_sequence<N>());
}
}
