#pragma once

#include <memory>

namespace khronos::detail
{
template <class Type, class Data, class... Args>
[[nodiscard]]
std::shared_ptr<Type> make_shared_with_data(Data && data, Args &&... args)
{
  auto deleter = [data = std::forward<Data>(data)](Type * p)
  {
    delete p;
  };

  Type * const ptr = new Type(std::forward<Args>(args)...);

  try
  {
    return {ptr, std::move(deleter)};
  }
  catch(...)
  {
    delete ptr;
    throw;
  }
}
}