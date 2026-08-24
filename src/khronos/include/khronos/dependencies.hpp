#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

namespace khronos
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

template <class... DependencyTypes>
requires(is_unique_types_v<DependencyTypes...>) class dependencies;

template <class DependentType, class Dependencies>
struct contains_dependency;

template <class DependentType, class... DependencyTypes>
struct contains_dependency<DependentType, dependencies<DependencyTypes...>>
: std::bool_constant<(std::is_same_v<DependentType, DependencyTypes> or ...)>
{
};

template <class DependentType>
struct contains_dependency<DependentType, void> : std::false_type
{
};

template <class DependentType, class Dependencies>
constexpr bool contains_dependency_v = contains_dependency<DependentType, Dependencies>::value;

template <class... DependencyTypes>
requires(is_unique_types_v<DependencyTypes...>) class dependencies
{
  public:
    template <class... OtherDependenciesType>
    constexpr dependencies(dependencies<OtherDependenciesType...> const & other)
      requires(sizeof...(DependencyTypes) == sizeof...(OtherDependenciesType))
    : ptrs(std::get<std::shared_ptr<DependencyTypes const>>(other.ptrs)...)
    {
    }

    template <class... OtherDependenciesType>
    constexpr dependencies & operator=(dependencies<OtherDependenciesType...> const & other)
      requires(sizeof...(DependencyTypes) == sizeof...(OtherDependenciesType))
    {
      ptrs = std::make_tuple(std::get<std::shared_ptr<DependencyTypes const>>(other.ptrs)...);

      return *this;
    }

    constexpr ~dependencies() noexcept = default;

    [[nodiscard]]
    constexpr auto operator<=>(dependencies const &) const noexcept = default;

    template <class Dependency>
    [[nodiscard]]
    constexpr auto const & get() const noexcept
    {
      return *std::get<std::shared_ptr<std::decay_t<Dependency> const>>(ptrs);
    }

    template <std::size_t I>
    [[nodiscard]]
    constexpr auto const & get() const noexcept
    {
      return *std::get<I>(ptrs);
    }

  private:
    template <class... OtherDependencyTypes>
    requires(is_unique_types_v<OtherDependencyTypes...>) friend class dependencies;

    template <class OtherDependentType, class OtherDependencies>
    requires(not contains_dependency_v<OtherDependentType, OtherDependencies>) friend class dependent;

    friend struct dependency_union_t;

    template <class OtherDependency>
    static constexpr bool contains = is_unique_types_v<OtherDependency, DependencyTypes...>;

    constexpr dependencies(std::tuple<std::shared_ptr<DependencyTypes const>...> const & other_ptrs) noexcept
    : ptrs(other_ptrs)
    {
      assert(std::apply([](auto const &... ptr) { return (ptr and ...); }, ptrs));
    }

    std::tuple<std::shared_ptr<DependencyTypes const>...> ptrs;
};

struct dependency_union_t
{
  private:
    template <class Dependencies,
              class RhsDependencyType,
              bool unique = Dependencies::template contains<RhsDependencyType>>
    struct union_dependency;

    template <class... LhsDependencyType, class RhsDependencyType>
    struct union_dependency<dependencies<LhsDependencyType...>, RhsDependencyType, true>
    {
        using type = dependencies<LhsDependencyType..., RhsDependencyType>;
    };

    template <class... LhsDependencyType, class RhsDependencyType>
    struct union_dependency<dependencies<LhsDependencyType...>, RhsDependencyType, false>
    {
        using type = dependencies<LhsDependencyType...>;
    };

    template <class Dependencies, class RhsDependencyType>
    using union_dependency_t = typename union_dependency<Dependencies, RhsDependencyType>::type;

    template <class Dependencies, class... OtherDependencyTypes>
    struct union_dependences;

    template <class Dependencies>
    struct union_dependences<Dependencies>
    {
        using type = Dependencies;
    };

    template <class Dependencies, class DependencyType, class... OtherDependencyTypes>
    struct union_dependences<Dependencies, DependencyType, OtherDependencyTypes...>
    {
        using type = typename union_dependences<union_dependency_t<Dependencies, DependencyType>,
                                                OtherDependencyTypes...>::type;
    };

  public:
    template <class... LhsDependencyTypes, class... RhsDependencyTypes>
    [[nodiscard]]
    constexpr auto operator()(dependencies<LhsDependencyTypes...> const & lhs,
                              dependencies<RhsDependencyTypes...> const & rhs) const

    {
      using result_type = typename union_dependences<dependencies<LhsDependencyTypes...>, RhsDependencyTypes...>::type;

      decltype(result_type::ptrs) result_ptrs;

      auto assign = [&]<class DependencyType>(std::shared_ptr<DependencyType const> const & ptr)
      {
        auto & result_ptr = std::get<std::shared_ptr<DependencyType const>>(result_ptrs);

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

inline constexpr auto dependency_union = dependency_union_t{0};

template <class DependentType, class Dependencies = void>
requires(not contains_dependency_v<DependentType, Dependencies>) class dependent
{
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

    template <class FunctionLike>
    struct function_traits : function_traits<decltype(&FunctionLike::operator())>
    {
    };

    template <class Return, class FunctionLike, class... Args>
    struct function_traits<Return (FunctionLike::*)(Args...) const> : function_traits<Return(Args...)>
    {
    };

    template <class FunctionLike>
    struct deleter_with_callback : Dependencies
    {
        constexpr deleter_with_callback(Dependencies const & deps, FunctionLike callback)
        : Dependencies(deps),
          callback(std::move(callback))
        {
        }

        constexpr void operator()(DependentType * p) const noexcept
        {
          // unlike std::unique_ptr, the deleter of std::shared_ptr is invoked even if the managed pointer is null.

          if(p)
          {
            [this, p]<std::size_t... I>(std::index_sequence<I...>)
            {
              callback(
                *p,
                this
                  ->template get<std::tuple_element_t<I + 1, typename function_traits<FunctionLike>::args_type>>()...);
            }(std::make_index_sequence<function_traits<FunctionLike>::arg_size - 1>{});

            std::default_delete<DependentType>{}(p);
          }
        }

        [[no_unique_address]]
        FunctionLike callback;
    };

    struct deleter_without_callback : std::default_delete<DependentType>, Dependencies
    {
        constexpr deleter_without_callback(Dependencies const & deps)
        : Dependencies(deps)
        {
        }
    };

  public:
    template <class FunctionLike, class... Args>
    constexpr dependent(FunctionLike callback, Dependencies const & deps, Args &&... args)
    : ptr(std::make_unique<DependentType>(std::forward<Args>(args)...).release(),
          deleter_with_callback<FunctionLike>{deps, callback}),
      deps(std::get_deleter<deleter_with_callback<FunctionLike>>(ptr))
    {
    }

    template <class... Args>
    constexpr dependent(Dependencies deps, Args &&... args)
    : ptr(std::make_unique<DependentType>(std::forward<Args>(args)...).release(), deleter_without_callback{deps}),
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

    template <class Dependency>
    [[nodiscard]]
    constexpr auto const & get_dependency() const noexcept
    {
      return deps->template get<Dependency>();
    }

    [[nodiscard]]
    constexpr auto as_dependencies() const noexcept
    {
      return dependency_union(dependencies<DependentType>{std::tuple{ptr}}, *deps);
    }

    [[nodiscard]]
    constexpr auto & get() & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr auto && get() && noexcept
    {
      return std::move(*ptr);
    }

    [[nodiscard]]
    constexpr auto const & get() const & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr operator DependentType &() & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr operator DependentType &&() && noexcept
    {
      return std::move(*ptr);
    }

    [[nodiscard]]
    constexpr operator DependentType const &() const & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr bool valueless_after_move() const noexcept
    {
      return not static_cast<bool>(ptr);
    }

  private:
    std::shared_ptr<DependentType> ptr;
    Dependencies *                 deps;
};

template <class DependentType>
class dependent<DependentType, void>
{
  private:
    template <class FunctionLike>
    struct deleter_with_callback
    {
        constexpr deleter_with_callback(FunctionLike callback)
        : callback(std::move(callback))
        {
        }

        constexpr void operator()(DependentType * p) const noexcept
        {
          // unlike std::unique_ptr, the deleter of std::shared_ptr is invoked even if the managed pointer is null.

          if(p)
          {
            callback(*p);

            std::default_delete<DependentType>{}(p);
          }
        }

        [[no_unique_address]]
        FunctionLike callback;
    };

  public:
    template <class FunctionLike, class... Args>
    constexpr dependent(FunctionLike callback, Args &&... args)
      requires(std::is_invocable_v<FunctionLike, DependentType &>)
    : ptr(std::make_unique<DependentType>(std::forward<Args>(args)...).release(),
          deleter_with_callback<FunctionLike>{callback})
    {
    }

    template <class... Args>
    constexpr dependent(Args &&... args)
    : ptr(std::make_shared<DependentType>(std::forward<Args>(args)...))
    {
    }

    constexpr dependent(dependent &&) noexcept = default;

    constexpr dependent(dependent const &) noexcept = delete;

    constexpr dependent & operator=(dependent &&) noexcept = default;

    constexpr dependent & operator=(dependent const &) noexcept = delete;

    [[nodiscard]]
    constexpr auto as_dependencies() const noexcept
    {
      return dependencies<DependentType>{std::tuple{ptr}};
    }

    [[nodiscard]]
    constexpr auto & get() & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr auto && get() && noexcept
    {
      return std::move(*ptr);
    }

    [[nodiscard]]
    constexpr auto const & get() const & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr operator DependentType &() & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr operator DependentType &&() && noexcept
    {
      return std::move(*ptr);
    }

    [[nodiscard]]
    constexpr operator DependentType const &() const & noexcept
    {
      return *ptr;
    }

    [[nodiscard]]
    constexpr bool valueless_after_move() const noexcept
    {
      return not static_cast<bool>(ptr);
    }

  private:
    std::shared_ptr<DependentType> ptr;
};
}