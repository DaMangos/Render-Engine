#pragma once

#include <cassert>
#include <functional>
#include <list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>

namespace khronos::detail
{
template <class Type>
struct deleter_with_data
{
    std::unordered_set<std::shared_ptr<void const>> data;
    std::list<std::function<void(Type &)>>          functions;

    void operator()(Type * const ptr) noexcept
    {
      for(auto & function : functions)
      {
        try
        {
          function(*ptr);
        }
        catch(...)
        {
        }
      }

      std::default_delete<Type>{}(ptr);
    }
};

template <class Type>
[[nodiscard]]
std::shared_ptr<Type> make_shared_with_data(auto &&... args)
{
  return {std::make_unique<Type>(std::forward<decltype(args)>(args)...).release(), deleter_with_data<Type>{}};
}

template <class Type, class Data>
void emplace_data(std::shared_ptr<Type> const & des, std::shared_ptr<Data> const & data)
{
  if(auto * deleter = std::get_deleter<deleter_with_data<std::add_const_t<Type>>>(des); deleter)
  {
    deleter->data.emplace(data);
    return;
  }

  if(auto * deleter = std::get_deleter<deleter_with_data<std::remove_const_t<Type>>>(des); deleter)
  {
    deleter->data.emplace(data);
    return;
  }

  throw std::logic_error("failed to emplace data");
}

template <class Type, class Data>
void erase_data(std::shared_ptr<Type> const & des, std::shared_ptr<Data> const & data)
{
  if(auto * deleter = std::get_deleter<deleter_with_data<std::add_const_t<Type>>>(des); deleter)
  {
    deleter->data.erase(data);
    return;
  }

  if(auto * deleter = std::get_deleter<deleter_with_data<std::remove_const_t<Type>>>(des); deleter)
  {
    deleter->data.erase(data);
    return;
  }

  throw std::logic_error("failed to erase data");
}

template <class Type>
void emplace_function(std::shared_ptr<Type> const & des, std::invocable<Type &> auto function)
{
  if(auto * deleter = std::get_deleter<deleter_with_data<std::add_const_t<Type>>>(des); deleter)
  {
    deleter->functions.emplace_back(std::move(function));
    return;
  }

  if(auto * deleter = std::get_deleter<deleter_with_data<std::remove_const_t<Type>>>(des); deleter)
  {
    deleter->functions.emplace_back(std::move(function));
    return;
  }

  throw std::logic_error("failed to erase function");
}
}