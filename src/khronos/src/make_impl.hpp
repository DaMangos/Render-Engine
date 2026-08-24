#pragma once

#include <memory>

namespace khronos
{
template <class Type, class... Args>
[[nodiscard]]
std::unique_ptr<Type, void (*)(Type *)> make_impl(Args &&... args)
{
  return {std::make_unique<Type>(std::forward<Args>(args)...).release(),
          [](Type * ptr) { std::default_delete<Type>{}(ptr); }};
}
}
