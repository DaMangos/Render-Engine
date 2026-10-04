#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace graphics
{
struct mesh_view;
struct const_mesh_view;
struct mesh;
class mesh_buffer;

namespace detail
{
template <bool Const>
class mesh_buffer_iterator
{
  public:
    using value_type        = mesh;
    using difference_type   = std::ptrdiff_t;
    using pointer           = void;
    using reference         = std::conditional_t<Const, const_mesh_view, mesh_view>;
    using iterator_category = std::random_access_iterator_tag;
    using iterator_concept  = std::random_access_iterator_tag;

    constexpr mesh_buffer_iterator() noexcept = default;

    template <class MeshBuffer>
    constexpr mesh_buffer_iterator(MeshBuffer & mesh_buffer, difference_type const index) noexcept
    : mesh_buffer(&mesh_buffer),
      index(index)
    {
    }

    [[nodiscard]]
    constexpr reference operator*() const noexcept
    {
      return {
        mesh_buffer->vertices(static_cast<std::size_t>(index)),
        mesh_buffer->indices(static_cast<std::size_t>(index)),
        mesh_buffer->texture(static_cast<std::size_t>(index)),
      };
    }

    constexpr mesh_buffer_iterator & operator++() noexcept
    {
      ++index;
      return *this;
    }

    constexpr mesh_buffer_iterator operator++(int) noexcept
    {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    constexpr mesh_buffer_iterator & operator--() noexcept
    {
      --index;
      return *this;
    }

    constexpr mesh_buffer_iterator operator--(int) noexcept
    {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    constexpr mesh_buffer_iterator & operator+=(difference_type i) noexcept
    {
      index += i;
      return *this;
    }

    constexpr mesh_buffer_iterator & operator-=(difference_type i) noexcept
    {
      index -= i;
      return *this;
    }

    [[nodiscard]]
    constexpr reference operator[](difference_type i) const noexcept
    {
      return *(*this + i);
    }

    template <bool OtherConst>
    [[nodiscard]]
    constexpr std::common_type_t<difference_type, typename mesh_buffer_iterator<OtherConst>::difference_type> operator-(
      mesh_buffer_iterator<OtherConst> const & other) noexcept
    {
      return index - other.index;
    }

    template <bool OtherConst>
    [[nodiscard]]
    constexpr auto operator==(mesh_buffer_iterator<OtherConst> const & other) const noexcept
    {
      return mesh_buffer == other.mesh_buffer and index == other.index;
    }

    template <bool OtherConst>
    [[nodiscard]]
    constexpr auto operator<=>(mesh_buffer_iterator<OtherConst> const & other) const noexcept
    {
      return index <=> other.index;
    }

    template <bool OtherConst>
    friend class mesh_buffer_iterator;

  private:
    std::conditional_t<Const, mesh_buffer const *, mesh_buffer *> mesh_buffer = {};
    difference_type                                               index       = {};
};

template <bool Const>
[[nodiscard]]
constexpr mesh_buffer_iterator<Const> operator+(mesh_buffer_iterator<Const>                           iter,
                                                typename mesh_buffer_iterator<Const>::difference_type i) noexcept
{
  iter += i;
  return iter;
}

template <bool Const>
[[nodiscard]]
constexpr mesh_buffer_iterator<Const> operator+(typename mesh_buffer_iterator<Const>::difference_type i,
                                                mesh_buffer_iterator<Const>                           iter) noexcept
{
  iter += i;
  return iter;
}

template <bool Const>
[[nodiscard]]
constexpr mesh_buffer_iterator<Const> operator-(mesh_buffer_iterator<Const>                           iter,
                                                typename mesh_buffer_iterator<Const>::difference_type i) noexcept
{
  return iter + (-i);
}
}
}
