#pragma once

#include <tuple/tuple_like.hpp>

#include <tuple>
#include <type_traits>

namespace tuple
{
template <class Tuple, class UnaryFunc>
requires(tuple_like<std::decay_t<Tuple>>)
constexpr void for_each(Tuple && tuple, UnaryFunc func)
{
  [&]<std::size_t... I>(std::index_sequence<I...>)
  {
    (func(std::get<I>(std::forward<Tuple>(tuple))), ...);
  }(std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>());
}
}
