#pragma once

#include <tuple/tuple_like.hpp>

#include <functional>
#include <utility>

namespace tuple
{
template <class Tuple, class Init, class BinaryOperation>
requires(tuple_like<std::decay_t<Tuple>>)
[[nodiscard]]
constexpr Init accumulate(Tuple && tuple, Init init, BinaryOperation op)
{
  [&]<std::size_t... I>(std::index_sequence<I...>)
  {
    return ((init = op(std::move(init), std::get<I>(std::forward<Tuple>(tuple)))), ...);
  }(std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>());

  return init;
}

template <class Tuple, class Init>
requires(tuple_like<std::decay_t<Tuple>>)
[[nodiscard]]
constexpr Init accumulate(Tuple && tuple, Init init)
{
  return accumulate(std::forward<Tuple>(tuple), std::move(init), std::plus<>{});
}
}
