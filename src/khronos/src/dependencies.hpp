#pragma once

/*
motivation

Vulkan objects have strict lifetime dependencies. For example, a "vk::raii::Instance" cannot
be destroyed until all the "vk::raii::Device"s it created, have been destroyed, A "vk::raii::Device"
cannot be destroyed until all the "vk::raii::Queue"s it created, have been destroyed, and so on...

To solve this issue we created the classes "khronos::dependencies" and "khronos::dependent"
along with the helper function "khronos::dependency_union". These classes allow you to be explict
in the lifetime dependencies of a given object. For example, take our "vk::raii::Queue", this depends
on a "vk::raii::Device", "vk::raii::PhysicalDevice", "vk::raii::Instance", and "vk::raii::Context".
Therefore, we can create the types:

  khronos::dependent<vk::raii::Context>

  khronos::dependent<vk::raii::Instance, khronos::dependencies<vk::raii::Context>

  khronos::dependent<vk::raii::PhysicalDevice, khronos::dependencies<vk::raii::Instance,
                                                                     vk::raii::Context>

  khronos::dependent<vk::raii::Device, khronos::dependencies<vk::raii::PhysicalDevice,
                                                             vk::raii::Instance,
                                                             vk::raii::Context>

  khronos::dependent<vk::raii::Queue, khronos::dependencies<vk::raii::Device,
                                                            vk::raii::PhysicalDevice,
                                                            vk::raii::Instance,
                                                            vk::raii::Context>


The "vk_context" is essentially a "std::shared_ptr<vk::raii::Context>". The "vk_instance" is essentially a
"std::shared_ptr<vk::raii::Instance>" with a "std::shared_ptr<vk::raii::Context>" imbedded in the deleter.
The "vk_physical_device" is essentially a "std::shared_ptr<vk::raii::PhysicalDevice>" with a
"std::shared_ptr<vk::raii::Instance>" and a "std::shared_ptr<vk::raii::Context>" imbedded in the deleter,
and so on and so fourth. This chain of "std::shared_ptr"s guarantees that a "vk::raii::Instance" cannot be
destroyed until every dependent has been destroyed.


synopsis

namespace khronos
{
template <class... Elements>
requires(is-unique-types<Elements...>)
class dependencies
{
  public:
    using as_dependencies_type = decltype(std::declval<dependent>().as_dependencies());

    template <class... OtherElement>
    constexpr dependencies(dependencies<OtherElement...> const & other)
      requires(sizeof...(Elements) == sizeof...(OtherElement));

    template <class... OtherElement>
    constexpr dependencies & operator=(dependencies<OtherElement...> const & other)
      requires(sizeof...(Elements) == sizeof...(OtherElement));

    [[nodiscard]]
    constexpr auto operator<=>(dependencies const &) const noexcept;

    template <class Dependency>
    [[nodiscard]]
    constexpr Dependency const & get() const noexcept;

    template <std::size_t I>
    [[nodiscard]]
    constexpr auto const & get() const noexcept;
};

struct dependency-union-type
{
    template <class... LhsElements, class... RhsElements>
    [[nodiscard]]
    constexpr auto operator()(dependencies<LhsElements...> const & lhs,
                              dependencies<RhsElements...> const & rhs) const;
};

inline constexpr dependency-union-type dependency_union;

template <class LhsDependencies, class RhsLhsDependencies>
using dependency_union_type = decltype(dependency_union(std::declval<LhsDependencies>(),
                                                        std::declval<RhsLhsDependencies>()));

template <class Element, class Dependencies = void>
requires(not contains-dependency<Element, Dependencies>)
class dependent
{
  public:
    using element_type         = Element;
    using dependencies_type    = Dependencies;
    using as_dependencies_type = as-dependencies-type;

    template <class FunctionLike, class... Args>
    constexpr dependent(FunctionLike callback, Dependencies const & deps, Args &&... args);

    template <class... Args>
    constexpr dependent(Dependencies deps, Args &&... args);

    constexpr dependent(dependent const &) noexcept = delete;

    constexpr dependent(dependent && other) noexcept;

    constexpr dependent & operator=(dependent const &) noexcept = delete;

    constexpr dependent & operator=(dependent && other) noexcept;

    template <class DependencyElement>
    [[nodiscard]]
    constexpr DependencyElement const & get_dependency() const noexcept;

    template <std::size_t I>
    [[nodiscard]]
    constexpr auto const & get_dependency() const noexcept

    [[nodiscard]]
    constexpr dependencies_type get_dependencies() const noexcept

    [[nodiscard]]
    constexpr as_dependencies_type as_dependencies() const noexcept;

    [[nodiscard]]
    constexpr element_type & get() & noexcept;

    [[nodiscard]]
    constexpr element_type && get() && noexcept;

    [[nodiscard]]
    constexpr element_type const & get() const & noexcept;

    [[nodiscard]]
    constexpr operator element_type &() & noexcept;

    [[nodiscard]]
    constexpr operator element_type &&() && noexcept;

    [[nodiscard]]
    constexpr operator element_type const &() const & noexcept;

    [[nodiscard]]
    constexpr bool valueless_after_move() const noexcept;
};

template <class Element>
class dependent<Element, void>
{
  public:
    using element_type         = Element;
    using as_dependencies_type = as-dependencies-type;

    template <class FunctionLike, class... Args>
    constexpr dependent(FunctionLike callback, Args &&... args)
      requires(std::is_invocable_v<FunctionLike, Element &>);

    template <class... Args>
    constexpr dependent(Args &&... args);

    constexpr dependent(dependent &&) noexcept;

    constexpr dependent(dependent const &) noexcept = delete;

    constexpr dependent & operator=(dependent &&) noexcept;

    constexpr dependent & operator=(dependent const &) noexcept = delete;

    [[nodiscard]]
    constexpr as_dependencies_type as_dependencies() const noexcept;

    [[nodiscard]]
    constexpr element_type & get() & noexcept;

    [[nodiscard]]
    constexpr element_type && get() && noexcept;

    [[nodiscard]]
    constexpr element_type const & get() const & noexcept;

    [[nodiscard]]
    constexpr operator element_type &() & noexcept;

    [[nodiscard]]
    constexpr operator element_type &&() && noexcept;

    [[nodiscard]]
    constexpr operator element_type const &() const & noexcept;

    [[nodiscard]]
    constexpr bool valueless_after_move() const noexcept;
};
}

template <std::size_t I, class... Elements>
struct std::tuple_element<I, khronos::dependencies<Elements...>>;
*/

#include <cassert>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

namespace khronos
{
namespace detail
{
template <class... Types>
struct is_unique_types;

template <>
struct is_unique_types<> : std::true_type
{
};

template <class Type>
struct is_unique_types<Type> : std::true_type
{
};

template <class Type, class... Types>
struct is_unique_types<Type, Types...>
: std::bool_constant<((not std::is_same_v<Type, Types>) and ...) and is_unique_types<Types...>::value>
{
};

template <class... Types>
constexpr bool is_unique_types_v = is_unique_types<Types...>::value;
}

template <class... Elements>
requires(detail::is_unique_types_v<Elements...>)
class dependencies;

namespace detail
{
template <class Dependencies, class Element>
struct contains_dependency;

template <class Element, class... Elements>
struct contains_dependency<dependencies<Elements...>, Element> : std::bool_constant<(std::is_same_v<Element, Elements> or ...)>
{
};

template <class Element>
struct contains_dependency<void, Element> : std::false_type
{
};

template <class Dependencies, class Element>
constexpr bool contains_dependency_v = detail::contains_dependency<Dependencies, Element>::value;

struct dependency_union_t;
}

template <class... Elements>
requires(detail::is_unique_types_v<Elements...>)
class dependencies
{
  public:
    template <class... OtherElement>
    constexpr dependencies(dependencies<OtherElement...> const & other) requires(sizeof...(Elements) == sizeof...(OtherElement))
    : ptrs(std::get<std::shared_ptr<Elements const>>(other.ptrs)...)
    {
    }

    template <class... OtherElement>
    constexpr dependencies & operator=(dependencies<OtherElement...> const & other)
      requires(sizeof...(Elements) == sizeof...(OtherElement))
    {
      ptrs = std::make_tuple(std::get<std::shared_ptr<Elements const>>(other.ptrs)...);

      return *this;
    }

    constexpr ~dependencies() noexcept = default;

    [[nodiscard]]
    constexpr auto operator<=>(dependencies const &) const noexcept = default;

    template <class Element>
    [[nodiscard]]
    constexpr Element const & get() const noexcept
    {
      return *std::get<std::shared_ptr<std::decay_t<Element> const>>(ptrs);
    }

    template <std::size_t I>
    [[nodiscard]]
    constexpr auto const & get() const noexcept
    {
      return *std::get<I>(ptrs);
    }

  private:
    template <class... OtherElements>
    requires(detail::is_unique_types_v<OtherElements...>)
    friend class dependencies;

    template <class OtherElement, class OtherDependencies>
    requires(not detail::contains_dependency_v<OtherDependencies, OtherElement>)
    friend class dependent;

    friend struct detail::dependency_union_t;

    template <class OtherDependency>
    static constexpr bool contains = detail::is_unique_types_v<OtherDependency, Elements...>;

    constexpr dependencies(std::tuple<std::shared_ptr<Elements const>...> const & other_ptrs) noexcept
    : ptrs(other_ptrs)
    {
      assert(std::apply([](auto const &... ptr) { return (ptr and ...); }, ptrs));
    }

    std::tuple<std::shared_ptr<Elements const>...> ptrs;
};

namespace detail
{
struct dependency_union_t
{
  private:
    template <class Dependencies, class Element, bool unique = Dependencies::template contains<Element>>
    struct append_element;

    template <class... Elements, class Element>
    struct append_element<dependencies<Elements...>, Element, true>
    {
        using type = dependencies<Elements..., Element>;
    };

    template <class... Elements, class Element>
    struct append_element<dependencies<Elements...>, Element, false>
    {
        using type = dependencies<Elements...>;
    };

    template <class Dependencies, class Element>
    using append_element_t = typename append_element<Dependencies, Element>::type;

    template <class Dependencies, class... Elements>
    struct append_elements;

    template <class Dependencies>
    struct append_elements<Dependencies>
    {
        using type = Dependencies;
    };

    template <class Dependencies, class Element, class... Elements>
    struct append_elements<Dependencies, Element, Elements...>
    {
        using type = typename append_elements<append_element_t<Dependencies, Element>, Elements...>::type;
    };

    template <class Dependencies, class... Elements>
    using append_elements_t = typename append_elements<Dependencies, Elements...>::type;

  public:
    template <class... LhsElements, class... RhsElements>
    [[nodiscard]]
    constexpr auto operator()(dependencies<LhsElements...> const & lhs, dependencies<RhsElements...> const & rhs) const

    {
      using result_type = append_elements_t<dependencies<LhsElements...>, RhsElements...>;

      decltype(result_type::ptrs) result_ptrs;

      auto assign = [&]<class Element>(std::shared_ptr<Element const> const & ptr)
      {
        auto & result_ptr = std::get<std::shared_ptr<Element const>>(result_ptrs);

        if(result_ptr and result_ptr != ptr)
          throw std::logic_error("dependency_union: conflicting dependencies");

        result_ptr = ptr;
      };

      std::apply([&](auto const &... ptr) { (assign(ptr), ...); }, rhs.ptrs);
      std::apply([&](auto const &... ptr) { (assign(ptr), ...); }, lhs.ptrs);

      return result_type{result_ptrs};
    }

    explicit constexpr dependency_union_t(int) noexcept
    {
    }
};
}

inline constexpr auto dependency_union = detail::dependency_union_t{0};

template <class LhsDependencies, class RhsLhsDependencies>
using dependency_union_type = decltype(dependency_union(std::declval<LhsDependencies>(), std::declval<RhsLhsDependencies>()));

template <class Element, class Dependencies = void>
requires(not detail::contains_dependency_v<Dependencies, Element>)
class dependent
{
  public:
    using element_type         = Element;
    using dependencies_type    = Dependencies;
    using as_dependencies_type = decltype(dependency_union(std::declval<dependencies<element_type>>(),
                                                           std::declval<dependencies_type>()));

  private:
    template <class FunctionLike>
    struct function_traits;

    template <class Return, class... Args>
    struct function_traits<Return(Args...)>
    {
        using return_type = Return;
        using args_type   = std::tuple<Args...>;

        static constexpr std::size_t arg_size = sizeof...(Args);
    };

    template <class Return, class... Args>
    struct function_traits<Return (*)(Args...)> : function_traits<Return(Args...)>
    {
    };

    template <class Return, class... Args>
    struct function_traits<Return (*)(Args...) noexcept> : function_traits<Return(Args...)>
    {
    };

    template <class FunctionLike>
    struct function_traits : function_traits<decltype(&FunctionLike::operator())>
    {
    };

    template <class Return, class FunctionLike, class... Args>
    struct function_traits<Return (FunctionLike::*)(Args...) const> : function_traits<Return(Args...)>
    {
    };

    template <class FunctionLike>
    struct deleter_with_callback : dependencies_type
    {
        constexpr deleter_with_callback(dependencies_type const & deps, FunctionLike callback)
        : Dependencies(deps),
          callback(std::move(callback))
        {
        }

        constexpr void operator()(element_type * p) const noexcept
        {
          // unlike std::unique_ptr, the deleter of std::shared_ptr is invoked even if the managed pointer is null.

          if(p)
          {
            [this, p]<std::size_t... I>(std::index_sequence<I...>)
            {
              callback(*p,
                       this->template get<std::tuple_element_t<I + 1, typename function_traits<FunctionLike>::args_type>>()...);
            }(std::make_index_sequence<function_traits<FunctionLike>::arg_size - 1>{});

            std::default_delete<element_type>{}(p);
          }
        }

        [[no_unique_address]]
        FunctionLike callback;
    };

    struct deleter_without_callback : std::default_delete<Element>, Dependencies
    {
        constexpr deleter_without_callback(Dependencies const & deps)
        : Dependencies(deps)
        {
        }
    };

  public:
    template <class FunctionLike, class... Args>
    constexpr dependent(FunctionLike callback, dependencies_type const & deps, Args &&... args)
    : ptr(std::make_unique<element_type>(std::forward<Args>(args)...).release(),
          deleter_with_callback<FunctionLike>{deps, callback}),
      deps(std::get_deleter<deleter_with_callback<FunctionLike>>(ptr))
    {
    }

    template <class... Args>
    constexpr dependent(dependencies_type deps, Args &&... args)
    : ptr(std::make_unique<element_type>(std::forward<Args>(args)...).release(), deleter_without_callback{deps}),
      deps(std::get_deleter<deleter_without_callback>(ptr))
    {
    }

    constexpr dependent(dependent const &) noexcept = delete;

    constexpr dependent(dependent && other) noexcept
    : ptr(std::move(other.ptr)),
      deps(std::exchange(other.deps, nullptr))
    {
    }

    constexpr dependent & operator=(dependent const &) noexcept = delete;

    constexpr dependent & operator=(dependent && other) noexcept
    {
      ptr  = std::move(other.ptr);
      deps = std::exchange(other.deps, nullptr);

      return *this;
    }

    template <class DependencyElement>
    [[nodiscard]]
    constexpr DependencyElement const & get_dependency() const noexcept
    {
      return deps->template get<DependencyElement>();
    }

    template <std::size_t I>
    [[nodiscard]]
    constexpr auto const & get_dependency() const noexcept
    {
      return deps->template get<I>();
    }

    [[nodiscard]]
    constexpr dependencies_type get_dependencies() const noexcept
    {
      return *deps;
    }

    [[nodiscard]]
    constexpr as_dependencies_type as_dependencies() const noexcept
    {
      return dependency_union(dependencies<element_type>{std::tuple{ptr}}, *deps);
    }

    [[nodiscard]]
    constexpr element_type & get() & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr element_type && get() && noexcept
    {
      return std::move(*ptr);
    }

    [[nodiscard]]
    constexpr element_type const & get() const & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr operator element_type &() & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr operator element_type &&() && noexcept
    {
      return std::move(*ptr);
    }

    [[nodiscard]]
    constexpr operator element_type const &() const & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr bool valueless_after_move() const noexcept
    {
      return not static_cast<bool>(ptr);
    }

  private:
    std::shared_ptr<element_type> ptr;
    dependencies_type *           deps;
};

template <class Element>
class dependent<Element, void>
{
  public:
    using element_type         = Element;
    using as_dependencies_type = dependencies<element_type>;

  private:
    template <class FunctionLike>
    struct deleter_with_callback
    {
        constexpr deleter_with_callback(FunctionLike callback)
        : callback(std::move(callback))
        {
        }

        constexpr void operator()(element_type * p) const noexcept
        {
          // unlike std::unique_ptr, the deleter of std::shared_ptr is invoked even if the managed pointer is null.

          if(p)
          {
            callback(*p);

            std::default_delete<element_type>{}(p);
          }
        }

        [[no_unique_address]]
        FunctionLike callback;
    };

  public:
    template <class FunctionLike, class... Args>
    constexpr dependent(FunctionLike callback, Args &&... args) requires(std::is_invocable_v<FunctionLike, element_type &>)
    : ptr(std::make_unique<element_type>(std::forward<Args>(args)...).release(), deleter_with_callback<FunctionLike>{callback})
    {
    }

    template <class... Args>
    constexpr dependent(Args &&... args)
    : ptr(std::make_shared<element_type>(std::forward<Args>(args)...))
    {
    }

    constexpr dependent(dependent &&) noexcept = default;

    constexpr dependent(dependent const &) noexcept = delete;

    constexpr dependent & operator=(dependent &&) noexcept = default;

    constexpr dependent & operator=(dependent const &) noexcept = delete;

    [[nodiscard]]
    constexpr as_dependencies_type as_dependencies() const noexcept
    {
      return dependencies<element_type>{std::tuple{ptr}};
    }

    [[nodiscard]]
    constexpr element_type & get() & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr element_type && get() && noexcept
    {
      return std::move(*ptr);
    }

    [[nodiscard]]
    constexpr element_type const & get() const & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr operator element_type &() & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr operator element_type &&() && noexcept
    {
      return std::move(*ptr);
    }

    [[nodiscard]]
    constexpr operator element_type const &() const & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr bool valueless_after_move() const noexcept
    {
      return not static_cast<bool>(ptr);
    }

  private:
    std::shared_ptr<element_type> ptr;
};

template <class Element, class... Elements>
[[nodiscard]]
constexpr Element const & get(dependencies<Elements...> const & dependencies) noexcept
{
  return dependencies.template get<Element>();
}

template <std::size_t I, class... Elements>
[[nodiscard]]
constexpr auto const & get(dependencies<Elements...> const & dependencies) noexcept
{
  return dependencies.template get<I>();
}
}

template <class... Elements>
struct std::tuple_size<khronos::dependencies<Elements...>> : std::integral_constant<std::size_t, sizeof...(Elements)>
{
};

template <std::size_t I, class... Elements>
struct std::tuple_element<I, khronos::dependencies<Elements...>> : std::tuple_element<I, std::tuple<Elements const &...>>
{
};
