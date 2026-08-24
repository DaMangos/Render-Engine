#include <tuple/tuple_like.hpp>

#include <array>
#include <tuple>
#include <vector>

namespace
{
struct tuple_like_struct
{
    template <std::size_t I>
    friend int get(tuple_like_struct const &) noexcept
    {
      return 0;
    }
};
}

template <>
struct std::tuple_size<tuple_like_struct> : std::integral_constant<std::size_t, 1>
{
};

template <>
struct std::tuple_element<0, tuple_like_struct>
{
    using type = int;
};

static_assert(tuple::tuple_like<std::tuple<int, char>>);
static_assert(tuple::tuple_like<std::pair<int, char>>);
static_assert(tuple::tuple_like<std::array<int, 1>>);
static_assert(not tuple::tuple_like<std::vector<int>>);
static_assert(tuple::tuple_like<tuple_like_struct>);
