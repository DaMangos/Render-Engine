#pragma once

#include <tuple/tuple_like.hpp>

#include <functional>
#include <tuple>
#include <type_traits>

namespace tuple
{
template <class LhsTuple, class RhsTuple, class Init, class BinaryOp1, class BinaryOp2>
requires(tuple_like<std::decay_t<LhsTuple>> and tuple_like<std::decay_t<RhsTuple>>)
[[nodiscard]]
constexpr Init inner_product(LhsTuple && lhs, RhsTuple && rhs, Init init, BinaryOp1 op1, BinaryOp2 op2)
{
  [&]<std::size_t... I>(std::index_sequence<I...>)
  {
    ((init = op1(std::move(init),
                 op2(std::get<I>(std::forward<LhsTuple>(lhs)), std::get<I>(std::forward<RhsTuple>(rhs))))),
     ...);
  }(std::make_index_sequence<std::min(std::tuple_size_v<std::decay_t<LhsTuple>>,
                                      std::tuple_size_v<std::decay_t<RhsTuple>>)>());

  return init;
}

template <class LhsTuple, class RhsTuple, class Init>
requires(tuple_like<std::decay_t<LhsTuple>> and tuple_like<std::decay_t<RhsTuple>>)
[[nodiscard]]
constexpr Init inner_product(LhsTuple && lhs, RhsTuple && rhs, Init init)
{
  return inner_product(std::forward<LhsTuple>(lhs),
                       std::forward<RhsTuple>(rhs),
                       std::move(init),
                       std::plus<>{},
                       std::multiplies<>{});
}
}
