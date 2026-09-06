
#pragma once

#include <cstddef>
#include <iterator>
#include <memory>

namespace iterator
{
template <class Iter, typename std::iterator_traits<Iter>::difference_type Stride>
requires(std::random_access_iterator<Iter> and Stride > 0)
class strided_iterator
{
  public:
    using value_type        = typename std::iterator_traits<Iter>::value_type;
    using difference_type   = typename std::iterator_traits<Iter>::difference_type;
    using pointer           = typename std::iterator_traits<Iter>::pointer;
    using reference         = typename std::iterator_traits<Iter>::reference;
    using iterator_category = typename std::iterator_traits<Iter>::iterator_category;
    using iterator_concept  = std::random_access_iterator_tag;

  private:
    Iter iter = {};

  public:
    constexpr explicit strided_iterator(Iter iter) noexcept
    : iter(iter)
    {
    }

    constexpr strided_iterator() noexcept = default;

    template <class OtherIter>
    constexpr strided_iterator(strided_iterator<OtherIter, Stride> const & other) noexcept
      requires(std::convertible_to<OtherIter, Iter>)
    : iter(other.iter)
    {
    }

    template <class OtherIter>
    constexpr strided_iterator & operator=(strided_iterator<OtherIter, Stride> const & other) noexcept
      requires(std::convertible_to<OtherIter, Iter>)

    {
      iter = other.iter;

      return *this;
    }

    [[nodiscard]]
    constexpr reference operator*() const noexcept
    {
      return *iter;
    }

    [[nodiscard]]
    constexpr pointer operator->() const noexcept
    {
      return std::addressof(*iter);
    }

    constexpr strided_iterator & operator++() noexcept
    {
      *this += 1;
      return *this;
    }

    constexpr strided_iterator operator++(int) noexcept
    {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    constexpr strided_iterator & operator--() noexcept
    {
      *this += -1;
      return *this;
    }

    constexpr strided_iterator operator--(int) noexcept
    {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    constexpr strided_iterator & operator+=(difference_type i) noexcept
    {
      iter += i * Stride;
      return *this;
    }

    constexpr strided_iterator & operator-=(difference_type i) noexcept
    {
      *this += -i;
      return *this;
    }

    [[nodiscard]]
    constexpr reference operator[](difference_type i) const noexcept
    {
      return *(*this + i);
    }

    [[nodiscard]]
    constexpr auto operator<=>(strided_iterator const & other) const noexcept = default;

    [[nodiscard]]
    constexpr Iter base() const noexcept
    {
      return iter;
    }

    static constexpr difference_type stride() noexcept
    {
      return Stride;
    }

    template <class OtherIter, typename std::iterator_traits<OtherIter>::difference_type OtherStride>
    requires(std::random_access_iterator<OtherIter> and OtherStride > 0)
    friend class strided_iterator;
};

template <class Iter, typename std::iterator_traits<Iter>::difference_type Stride>
[[nodiscard]]
constexpr strided_iterator<Iter, Stride> operator+(strided_iterator<Iter, Stride>                           iter,
                                                   typename strided_iterator<Iter, Stride>::difference_type i) noexcept
{
  iter += i;
  return iter;
}

template <class Iter, typename std::iterator_traits<Iter>::difference_type Stride>
[[nodiscard]]
constexpr strided_iterator<Iter, Stride> operator+(typename strided_iterator<Iter, Stride>::difference_type i,
                                                   strided_iterator<Iter, Stride>                           iter) noexcept
{
  iter += i;
  return iter;
}

template <class LhsIter, class RhsIter, typename std::iterator_traits<LhsIter>::difference_type Stride>
[[nodiscard]]
constexpr
  typename strided_iterator<LhsIter, Stride>::difference_type operator-(strided_iterator<LhsIter, Stride> const & lhs,
                                                                        strided_iterator<RhsIter, Stride> const & rhs) noexcept
{
  return (lhs.base() - rhs.base()) / Stride;
}

template <class Iter, typename std::iterator_traits<Iter>::difference_type Stride>
[[nodiscard]]
constexpr strided_iterator<Iter, Stride> operator-(strided_iterator<Iter, Stride>                           iter,
                                                   typename strided_iterator<Iter, Stride>::difference_type i) noexcept
{
  return iter + (-i);
}

template <class LhsIter, class RhsIter, typename std::iterator_traits<LhsIter>::difference_type Stride>
[[nodiscard]]
constexpr auto operator<=>(strided_iterator<LhsIter, Stride> const & lhs,
                           strided_iterator<RhsIter, Stride> const & rhs) noexcept
{
  return lhs.base() <=> rhs.base();
}
}