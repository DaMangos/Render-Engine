
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

    constexpr explicit strided_iterator(Iter first) noexcept
    : first(first)
    {
    }

    constexpr strided_iterator() noexcept = default;

    template <class OtherIter>
    constexpr strided_iterator(strided_iterator<OtherIter, Stride> const & other) noexcept
      requires(std::convertible_to<OtherIter, Iter>)
    : first(other.first)
    {
    }

    template <class OtherIter>
    constexpr strided_iterator & operator=(strided_iterator<OtherIter, Stride> const & other) noexcept
      requires(std::convertible_to<OtherIter, Iter>)

    {
      first = other.first;

      return *this;
    }

    [[nodiscard]]
    constexpr reference operator*() const noexcept
    {
      return *std::ranges::next(first, offset);
    }

    [[nodiscard]]
    constexpr pointer operator->() const noexcept
    {
      return std::addressof(*std::ranges::next(first, offset));
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
      offset += (i * Stride);
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
      return std::ranges::next(first, offset);
    }

    static constexpr difference_type stride() noexcept
    {
      return Stride;
    }

    template <class OtherIter>
    [[nodiscard]]
    constexpr difference_type operator-(strided_iterator<OtherIter, Stride> const & other) const noexcept
    {
      return (offset - other.offset) / Stride;
    }

    template <class OtherIter, typename std::iterator_traits<OtherIter>::difference_type OtherStride>
    requires(std::random_access_iterator<OtherIter> and OtherStride > 0)
    friend class strided_iterator;

  private:
    Iter            first  = {};
    difference_type offset = {};

    // We keep a iterator and an offset and when we call operator* or operator-> we use increment
    // the iterator by the offset then return it. Furthermore, calls to operator++ operator-- etc
    // just increment the offset and leave the iterator untouched. This is because if we have a
    // range with 9 elements and our stride is 3 and we want to iterate over the 2nd, 5th, 8th
    // element, our end iterator would be the theoretical 11th element. In a constexpr environment
    // this is unacceptable and when we try and construct the end iterator get the complier error
    // (constexpr_var_requires_const_init) with the message:
    //    "Cannot refer to element 11 of array of 9 elements in a constant expression".
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