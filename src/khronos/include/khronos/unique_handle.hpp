
#include <cassert>
#include <cstddef>
#include <memory>
#include <tuple>
#include <utility>

namespace khronos
{
template <class Type, class LifetimeDependences>
class unique_handle;

template <class Type, class... Dependences>
class dependency_builder : private std::tuple<std::shared_ptr<Dependences>...>
{
  public:
    using cleanup_type = void (*)(Type &, Dependences &...) noexcept;

    constexpr dependency_builder() noexcept = default;

    template <class... OtherDependences>
    constexpr dependency_builder(OtherDependences const &... lifetime_dependences) noexcept
    : cleanup(nullptr)
    {
      (add_dependency(*lifetime_dependences), ...);
    }

    template <class... OtherDependences>
    constexpr dependency_builder(cleanup_type const cleanup, OtherDependences const &... lifetime_dependences) noexcept
    : cleanup(cleanup)
    {
      (add_dependency(*lifetime_dependences), ...);
    }

    constexpr dependency_builder(dependency_builder && other)
    : std::tuple<std::shared_ptr<Dependences>...>(
        std::move(static_cast<std::tuple<std::shared_ptr<Dependences>...> &&>(other)))
    {
      cleanup = std::exchange(other.cleanup, nullptr);
    }

    constexpr dependency_builder(dependency_builder const & other) = default;

    constexpr dependency_builder & operator=(dependency_builder && other)
    {
      static_cast<std::tuple<std::shared_ptr<Dependences>...> &>(*this) = std::move(
        static_cast<std::tuple<std::shared_ptr<Dependences>...> &&>(other));

      cleanup = std::exchange(other.cleanup, nullptr);

      return *this;
    }

    constexpr dependency_builder & operator=(dependency_builder const & other) = default;

    constexpr ~dependency_builder() = default;

    template <class Dependency, class LifetimeDependences>
    constexpr dependency_builder & add_dependency(unique_handle<Dependency, LifetimeDependences> const & dependency)
    {
      std::get<Dependency>(*this) = dependency.shared_ptr;

      return *this;
    }

    template <class Dependency>
    constexpr dependency_builder & add_dependency(std::shared_ptr<Dependency> const & dependency)
    {
      std::get<Dependency>(*this) = dependency;

      return *this;
    }

    constexpr dependency_builder & reset_cleanup(cleanup_type const other_cleanup) noexcept
    {
      cleanup = other_cleanup;

      return *this;
    }

    constexpr void invoke_cleanup(Type & value) noexcept
    {
      if(cleanup)
        [&, this]<std::size_t... I>(std::index_sequence<I...>)
        {
          cleanup(value, *std::get<I>(*this)...);
        }(std::make_index_sequence<sizeof...(Dependences)>{});
    }

    constexpr explicit operator bool() noexcept
    {
      return [this]<std::size_t... I>(std::index_sequence<I...>)
      {
        return (std::get<I>(*this) and ...);
      }(std::make_index_sequence<sizeof...(Dependences)>{});
    }

  private:
    cleanup_type cleanup = nullptr;
};

template <class... Dependences>
struct lifetime_dependences
{
    template <class Type>
    using dependency_builder_type = dependency_builder<Type, Dependences...>;

    lifetime_dependences() = delete;
};

struct no_dependences
{
    no_dependences() = delete;
};

struct nullhandle_t
{
    constexpr nullhandle_t(int) noexcept
    {
    }
};

inline constexpr auto nullhandle = nullhandle_t{0};

template <class Type, class LifetimeDependences>
class unique_handle
{
  public:
    using element_type            = Type;
    using dependency_builder_type = typename LifetimeDependences::template dependency_builder_type<element_type>;

  private:
    struct deleter_type
    {
        dependency_builder_type dependency_builder;

        constexpr void operator()(element_type * ptr) noexcept
        {
          if(not ptr)
            return;

          dependency_builder.invoke_cleanup(*ptr);

          std::default_delete<element_type>{}(ptr);
        }
    };

  public:
    constexpr unique_handle(nullhandle_t) noexcept
    : shared_ptr(nullptr)
    {
    }

    template <class... Args>
    constexpr unique_handle(dependency_builder_type const & dependency_builder, Args &&... args)
    : shared_ptr(std::make_unique<element_type>(std::forward<Args>(args)...).release(),
                 deleter_type{dependency_builder})
    {
      assert(dependency_builder);
    }

    constexpr unique_handle(unique_handle &&) noexcept = default;

    constexpr unique_handle(unique_handle const &) noexcept = delete;

    constexpr unique_handle & operator=(unique_handle &&) noexcept = default;

    constexpr unique_handle & operator=(unique_handle const &) noexcept = delete;

    constexpr unique_handle & operator=(nullhandle_t) noexcept
    {
      shared_ptr = nullptr;

      return *this;
    }

    constexpr ~unique_handle() = default;

    constexpr auto operator<=>(unique_handle const &) const noexcept = default;

    constexpr auto operator<=>(nullhandle_t) const noexcept
    {
      return shared_ptr <=> nullptr;
    }

    template <class... Args>
    constexpr void reset(dependency_builder_type const & dependency_builder, Args &&... args)
    {
      assert(dependency_builder);

      shared_ptr.reset(std::make_unique<element_type>(std::forward<Args>(args)...).release(),
                       deleter_type{dependency_builder});
    }

    [[nodiscard]]
    constexpr explicit operator bool() const noexcept
    {
      return static_cast<bool>(shared_ptr);
    }

    [[nodiscard]]
    constexpr decltype(auto) operator*() & noexcept
    {
      return *shared_ptr;
    }

    [[nodiscard]]
    constexpr decltype(auto) operator*() && noexcept
    {
      return *std::move(shared_ptr);
    }

    [[nodiscard]]
    constexpr decltype(auto) operator*() const & noexcept
    {
      return *shared_ptr;
    }

    [[nodiscard]]
    constexpr decltype(auto) operator->() & noexcept
    {
      return shared_ptr.operator->();
    }

    [[nodiscard]]
    constexpr decltype(auto) operator->() && noexcept
    {
      return std::move(shared_ptr).operator->();
    }

    [[nodiscard]]
    constexpr decltype(auto) operator->() const & noexcept
    {
      return shared_ptr.operator->();
    }

  private:
    template <class, class...>
    friend class dependency_builder;

    std::shared_ptr<Type> shared_ptr;
};

template <class Type>
class unique_handle<Type, no_dependences>
{
  public:
    using element_type            = Type;
    using dependency_builder_type = void;

    constexpr unique_handle(nullhandle_t) noexcept
    : shared_ptr(nullptr)
    {
    }

    template <class... Args>
    constexpr unique_handle(Args &&... args)
    : shared_ptr(std::make_shared<element_type>(std::forward<Args>(args)...))
    {
    }

    constexpr unique_handle(unique_handle &&) noexcept = default;

    constexpr unique_handle(unique_handle const &) noexcept = delete;

    constexpr unique_handle & operator=(unique_handle &&) noexcept = default;

    constexpr unique_handle & operator=(unique_handle const &) noexcept = delete;

    constexpr unique_handle & operator=(nullhandle_t) noexcept
    {
      shared_ptr = nullptr;

      return *this;
    }

    constexpr ~unique_handle() = default;

    constexpr auto operator<=>(unique_handle const &) const noexcept = default;

    constexpr auto operator<=>(nullhandle_t) const noexcept
    {
      return shared_ptr <=> nullptr;
    }

    template <class... Args>
    constexpr void reset(Args &&... args)
    {
      shared_ptr = std::make_shared<element_type>(std::forward<Args>(args)...);
    }

    [[nodiscard]]
    constexpr explicit operator bool() const noexcept
    {
      return static_cast<bool>(shared_ptr);
    }

    [[nodiscard]]
    constexpr decltype(auto) operator*() & noexcept
    {
      return *shared_ptr;
    }

    [[nodiscard]]
    constexpr decltype(auto) operator*() && noexcept
    {
      return *std::move(shared_ptr);
    }

    [[nodiscard]]
    constexpr decltype(auto) operator*() const & noexcept
    {
      return *shared_ptr;
    }

    [[nodiscard]]
    constexpr decltype(auto) operator->() & noexcept
    {
      return shared_ptr.operator->();
    }

    [[nodiscard]]
    constexpr decltype(auto) operator->() && noexcept
    {
      return std::move(shared_ptr).operator->();
    }

    [[nodiscard]]
    constexpr decltype(auto) operator->() const & noexcept
    {
      return shared_ptr.operator->();
    }

  private:
    std::shared_ptr<Type> shared_ptr;
};
}