#pragma once

/*
       synopsis

namespace maths
{
enum class layout
{
  row_major,
  column_major
};

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
class basic_column_view : public std::ranges::view_interface<basic_column_view<Iter, ColumnSize, RowSize, Layout>>
{
  public:
    template <class OtherIter>
    explicit constexpr basic_column_view(OtherIter iter) noexcept requires(std::convertible_to<OtherIter, Iter>);

    constexpr basic_column_view(basic_column_view const & other) noexcept;

    template <class OtherIter, std::size_t>
    constexpr basic_column_view(basic_column_view<OtherIter, ColumnSize, RowSize, Layout> const & other) noexcept
      requires(std::convertible_to<Iter, OtherIter>);

    template <class OtherIter, std::size_t OtherRowSize, layout OtherLayout>
    constexpr basic_column_view & operator=(
      basic_column_view<OtherIter, ColumnSize, OtherRowSize, OtherLayout> const &) noexcept;

    template <class OtherMatrix>
    constexpr basic_column_view & operator=(OtherMatrix const & mat) noexcept;

    constexpr basic_column_view & operator=(std::initializer_list<std::remove_const_t<std::iter_value_t<Iter>>>) noexcept;

    template <class OtherIter, std::sentinel_for<OtherIter> OtherSent>
    constexpr void assign(OtherIter first, OtherSent last);

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range);

    [[nodiscard]]
    constexpr auto begin() const noexcept;

    [[nodiscard]]
    constexpr auto end() const noexcept;

    [[nodiscard]]
    constexpr bool operator==(basic_column_view const & other) const noexcept;

    [[nodiscard]]
    constexpr bool operator!=(basic_column_view const & other) const noexcept;
};

template <class Matrix>
class basic_column_views : public std::ranges::view_interface<basic_column_views<Matrix>>
{
  public:
    explicit constexpr basic_column_views(Matrix & mat) noexcept;

    constexpr basic_column_views(basic_column_views const & other) noexcept;

    template <class OtherMatrix>
    constexpr basic_column_views(basic_column_views<OtherMatrix> const & other) noexcept;

    constexpr basic_column_views & operator=(basic_column_views const & other) noexcept;

    template <class OtherMatrix>
    constexpr basic_column_views & operator=(basic_column_views<OtherMatrix> const & other) noexcept
      requires(std::convertible_to<OtherMatrix, Matrix>);

    template <class OtherMatrix>
    constexpr basic_column_views & operator=(OtherMatrix const & other_mat) noexcept
      requires(std::convertible_to<OtherMatrix, Matrix>);

    template <class Range = std::array<typename Matrix::value_type, Matrix::row_size()>>
    constexpr basic_column_views & operator=(std::initializer_list<Range> list) noexcept;

    template <class Iter, std::sentinel_for<Iter> Sent>
    constexpr void assign(Iter first, Sent last);

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range);

    [[nodiscard]]
    constexpr auto begin() const noexcept;

    [[nodiscard]]
    constexpr auto end() const noexcept;

    [[nodiscard]]
    constexpr bool operator==(basic_column_views const & other) const noexcept;

    [[nodiscard]]
    constexpr bool operator!=(basic_column_views const & other) const noexcept;
};

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
class basic_row_view : public std::ranges::view_interface<basic_row_view<Iter, ColumnSize, RowSize, Layout>>
{
  public:
    template <class OtherIter>
    explicit constexpr basic_row_view(OtherIter iter) noexcept requires(std::convertible_to<OtherIter, Iter>);

    constexpr basic_row_view(basic_row_view const & other) noexcept;

    template <class OtherIter>
    constexpr basic_row_view(basic_row_view<OtherIter, ColumnSize, RowSize, Layout> const & other) noexcept
      requires(std::convertible_to<Iter, OtherIter>);

    template <class OtherIter, std::size_t OtherColumnSize, layout OtherLayout>
    constexpr basic_row_view & operator=(basic_row_view<OtherIter, OtherColumnSize, RowSize, OtherLayout> const &) noexcept;

    template <class OtherMatrix>
    constexpr basic_row_view & operator=(OtherMatrix const & mat) noexcept;

    constexpr basic_row_view & operator=(std::initializer_list<std::remove_const_t<std::iter_value_t<Iter>>>) noexcept;

    template <class OtherIter, std::sentinel_for<OtherIter> OtherSent>
    constexpr void assign(OtherIter first, OtherSent last);

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range);

    [[nodiscard]]
    constexpr auto begin() const noexcept;

    [[nodiscard]]
    constexpr auto end() const noexcept;

    [[nodiscard]]
    constexpr bool operator==(basic_row_view const & other) const noexcept;

    [[nodiscard]]
    constexpr bool operator!=(basic_row_view const & other) const noexcept;
};

template <class Matrix>
class basic_row_views : public std::ranges::view_interface<basic_row_views<Matrix>>
{
  public:
    explicit constexpr basic_row_views(Matrix & mat) noexcept;

    constexpr basic_row_views(basic_row_views const & other) noexcept;

    template <class OtherMatrix>
    constexpr basic_row_views(basic_row_views<OtherMatrix> const & other) noexcept;

    constexpr basic_row_views & operator=(basic_row_views const & other) noexcept;

    template <class OtherMatrix>
    constexpr basic_row_views & operator=(basic_row_views<OtherMatrix> const & other) noexcept
      requires(std::convertible_to<OtherMatrix, Matrix>);

    template <class OtherMatrix>
    constexpr basic_row_views & operator=(OtherMatrix const & other_mat) noexcept
      requires(std::convertible_to<OtherMatrix, Matrix>);

    template <class Range = std::array<typename Matrix::value_type, Matrix::row_size()>>
    constexpr basic_row_views & operator=(std::initializer_list<Range> list) noexcept;

    template <class Iter, std::sentinel_for<Iter> Sent>
    constexpr void assign(Iter first, Sent last);

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range);

    [[nodiscard]]
    constexpr auto begin() const noexcept;

    [[nodiscard]]
    constexpr auto end() const noexcept;

    [[nodiscard]]
    constexpr bool operator==(basic_row_views const & other) const noexcept;

    [[nodiscard]]
    constexpr bool operator!=(basic_row_views const & other) const noexcept;
};

template <class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout = layout::column_major>
requires(std::is_arithmetic_v<Arithmetic> and ColumnSize > 0uz and RowSize > 0uz
         and (Layout == layout::row_major or Layout == layout::column_major))
class matrix
{
  public:
    using value_type         = Arithmetic;
    using reference          = Arithmetic &;
    using const_reference    = Arithmetic const &;
    using pointer            = Arithmetic *;
    using const_pointer      = Arithmetic const *;
    using size_type          = std::size_t;
    using difference_type    = std::ptrdiff_t;
    using row_view           = basic_row_view<pointer, ColumnSize, RowSize, Layout>;
    using const_row_view     = basic_row_view<const_pointer, ColumnSize, RowSize, Layout>;
    using column_view        = basic_column_view<pointer, ColumnSize, RowSize, Layout>;
    using const_column_view  = basic_column_view<const_pointer, ColumnSize, RowSize, Layout>;
    using row_views          = basic_row_views<matrix>;
    using const_row_views    = basic_row_views<matrix const>;
    using column_views       = basic_column_views<matrix>;
    using const_column_views = basic_column_views<matrix const>;

    constexpr matrix() noexcept = default;

    template <class Iter, std::sentinel_for<Iter> Sent>
    constexpr matrix(Iter first, Sent last);

    template <std::ranges::input_range Range>
    constexpr matrix(std::from_range_t, Range && range);

    constexpr matrix(std::initializer_list<Arithmetic> list) noexcept;

    template <class OtherArithmetic, class Extents, class AccessorPolicy>
    constexpr matrix(std::mdspan<OtherArithmetic, Extents, std::layout_left, AccessorPolicy> const & md) noexcept
      requires(Extents::static_extent(0) == ColumnSize and Extents::static_extent(1) == RowSize);

    template <class OtherArithmetic, class Extents, class AccessorPolicy>
    constexpr matrix(std::mdspan<OtherArithmetic, Extents, std::layout_right, AccessorPolicy> const & md) noexcept
      requires(Extents::static_extent(0) == ColumnSize and Extents::static_extent(1) == RowSize);

    constexpr matrix(row_view const & view) noexcept requires(ColumnSize == 1);

    constexpr matrix(const_row_view const & view) noexcept requires(ColumnSize == 1);

    constexpr matrix(column_view const & view) noexcept requires(RowSize == 1);

    constexpr matrix(const_column_view const & view) noexcept requires(RowSize == 1);

    constexpr matrix(Arithmetic value) noexcept requires(ColumnSize == 1 and RowSize == 1);

    template <class OtherArithmetic, layout OtherLayout>
    constexpr matrix(matrix<OtherArithmetic, ColumnSize, RowSize, OtherLayout> const & other) noexcept;

    constexpr matrix & operator=(std::initializer_list<Arithmetic> list) noexcept;

    template <class Extents, class AccessorPolicy>
    constexpr matrix & operator=(std::mdspan<Arithmetic, Extents, std::layout_left, AccessorPolicy> const & md) noexcept
      requires(Extents::static_extent(0) == ColumnSize and Extents::static_extent(1) == RowSize);

    template <class Extents, class AccessorPolicy>
    constexpr matrix & operator=(std::mdspan<Arithmetic, Extents, std::layout_right, AccessorPolicy> const & md) noexcept
      requires(Extents::static_extent(0) == ColumnSize and Extents::static_extent(1) == RowSize);

    constexpr matrix & operator=(const_row_view const & view) noexcept requires(ColumnSize == 1);

    constexpr matrix & operator=(const_column_view const & view) noexcept requires(RowSize == 1);

    template <class OtherArithmetic, layout OtherLayout>
    constexpr matrix & operator=(matrix<OtherArithmetic, ColumnSize, RowSize, OtherLayout> const & other) noexcept;

    [[nodiscard]]
    constexpr operator std::mdspan<Arithmetic, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_right>() & noexcept
      requires(Layout == layout::row_major);

    [[nodiscard]]
    constexpr operator std::mdspan<Arithmetic, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_left>() & noexcept
      requires(Layout == layout::column_major);

    constexpr operator std::mdspan<Arithmetic, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_right>() && noexcept
      requires(Layout == layout::row_major)
      = delete;

    constexpr operator std::mdspan<Arithmetic, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_left>() && noexcept
      requires(Layout == layout::column_major)
      = delete;

    [[nodiscard]]
    constexpr operator std::mdspan<Arithmetic const, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_right>()
      const & noexcept requires(Layout == layout::row_major);

    [[nodiscard]]
    constexpr operator std::mdspan<Arithmetic const, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_left>()
      const & noexcept requires(Layout == layout::column_major);

    [[nodiscard]]
    constexpr operator Arithmetic() && noexcept requires(ColumnSize == 1uz and RowSize == 1uz);

    [[nodiscard]]
    constexpr operator Arithmetic &() & noexcept requires(ColumnSize == 1uz and RowSize == 1uz);

    [[nodiscard]]
    constexpr operator Arithmetic const &() const & noexcept requires(ColumnSize == 1uz and RowSize == 1uz);

    template <class Iter, std::sentinel_for<Iter> Sent>
    constexpr void assign(Iter first, Sent last);

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range);

    [[nodiscard]]
    static constexpr layout layout() noexcept;

    [[nodiscard]]
    constexpr pointer data() noexcept;

    [[nodiscard]]
    constexpr const_pointer data() const noexcept;

    [[nodiscard]]
    static constexpr size_type size() noexcept;

    [[nodiscard]]
    static constexpr difference_type ssize() noexcept;

    [[nodiscard]]
    static constexpr size_type column_size() noexcept;

    [[nodiscard]]
    static constexpr size_type row_size() noexcept;

    [[nodiscard]]
    static constexpr difference_type column_ssize() noexcept;

    [[nodiscard]]
    static constexpr difference_type row_ssize() noexcept;

    [[nodiscard]]
    static constexpr difference_type column_stride() noexcept;

    [[nodiscard]]
    static constexpr difference_type row_stride() noexcept;

    [[nodiscard]]
    constexpr row_views rows() noexcept;

    [[nodiscard]]
    constexpr const_row_views rows() const noexcept;

    [[nodiscard]]
    constexpr column_views columns() noexcept;

    [[nodiscard]]
    constexpr const_column_views columns() const noexcept;

    [[nodiscard]]
    constexpr row_view row(difference_type i) noexcept;

    [[nodiscard]]
    constexpr const_row_view row(difference_type i) const noexcept;

    [[nodiscard]]
    constexpr column_view column(difference_type j) noexcept;

    [[nodiscard]]
    constexpr const_column_view column(difference_type j) const noexcept;

    [[nodiscard]]
    constexpr row_view operator[](difference_type i) noexcept;

    [[nodiscard]]
    constexpr const_row_view operator[](difference_type i) const noexcept;

    [[nodiscard]]
    constexpr matrix operator-() noexcept;
};
}
*/

#include <iterator/strided_iterator.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <mdspan>
#include <numeric>
#include <ranges>
#include <type_traits>
#include <utility>

namespace maths
{
enum class layout
{
  row_major,
  column_major
};

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
class basic_column_view : public std::ranges::view_interface<basic_column_view<Iter, ColumnSize, RowSize, Layout>>
{
  public:
    template <class OtherIter>
    explicit constexpr basic_column_view(OtherIter const iter) noexcept requires(std::convertible_to<OtherIter, Iter>)
    : iter(iter)
    {
    }

    constexpr basic_column_view(basic_column_view const & other) noexcept
    : iter(other.iter)
    {
    }

    template <class OtherIter, std::size_t>
    constexpr basic_column_view(basic_column_view<OtherIter, ColumnSize, RowSize, Layout> const & other) noexcept
      requires(std::convertible_to<Iter, OtherIter>)
    : iter(other.iter)
    {
    }

    template <class OtherIter, std::size_t OtherRowSize, layout OtherLayout>
    constexpr basic_column_view & operator=(
      basic_column_view<OtherIter, ColumnSize, OtherRowSize, OtherLayout> const & other) noexcept
      requires(std::convertible_to<Iter, OtherIter>)
    {
      assign(std::from_range, other);

      return *this;
    }

    template <class OtherMatrix>
    constexpr basic_column_view & operator=(OtherMatrix const & mat) noexcept
      requires(std::decay_t<OtherMatrix>::row_size() == 1uz and std::decay_t<OtherMatrix>::column_size() == ColumnSize)
    {
      assign(std::from_range, mat.column(0));

      return *this;
    }

    constexpr basic_column_view & operator=(
      std::initializer_list<std::remove_const_t<std::iter_value_t<Iter>>> const list) noexcept
    {
      assign(std::from_range, list);

      return *this;
    }

    template <class OtherIter, std::sentinel_for<OtherIter> OtherSent>
    constexpr void assign(OtherIter const first, OtherSent const last)
    {
      std::ranges::copy_n(first, std::min(std::ranges::distance(first, last), std::ranges::distance(begin(), end())), begin());
    }

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range)
    {
      assign(std::forward<Range>(range).begin(), std::forward<Range>(range).end());
    }

    [[nodiscard]]
    constexpr auto begin() const noexcept
    {
      return iter;
    }

    [[nodiscard]]
    constexpr auto end() const noexcept
    {
      return std::ranges::next(iter, static_cast<std::iter_difference_t<Iter>>(ColumnSize));
    }

    [[nodiscard]]
    constexpr bool operator==(basic_column_view const & other) const noexcept
    {
      return std::ranges::equal(*this, other);
    }

    [[nodiscard]]
    constexpr bool operator!=(basic_column_view const & other) const noexcept
    {
      return not(*this == other);
    }

  private:
    iterator::strided_iterator<Iter, Layout == layout::column_major ? 1uz : RowSize> iter;
};

template <class Matrix>
class basic_column_views : public std::ranges::view_interface<basic_column_views<Matrix>>
{
  public:
    explicit constexpr basic_column_views(Matrix & mat) noexcept
    : mat(&mat),
      view(std::ranges::iota_view{static_cast<typename Matrix::difference_type>(0), Matrix::row_ssize()}, invoke_column{this})
    {
    }

    constexpr basic_column_views(basic_column_views const & other) noexcept
    : mat(other.mat),
      view(std::ranges::iota_view{static_cast<typename Matrix::difference_type>(0), Matrix::row_ssize()}, invoke_column{this})
    {
    }

    template <class OtherMatrix>
    constexpr basic_column_views(basic_column_views<OtherMatrix> const & other) noexcept
    : mat(other.mat),
      view(std::ranges::iota_view{static_cast<typename Matrix::difference_type>(0), Matrix::row_ssize()}, invoke_column{this})
    {
    }

    constexpr basic_column_views & operator=(basic_column_views const & other) noexcept
    {
      assign(std::from_range, other);

      return *this;
    }

    template <class OtherMatrix>
    constexpr basic_column_views & operator=(basic_column_views<OtherMatrix> const & other) noexcept
      requires(std::convertible_to<OtherMatrix, Matrix>)
    {
      assign(std::from_range, other);

      return *this;
    }

    template <class OtherMatrix>
    constexpr basic_column_views & operator=(OtherMatrix const & other_mat) noexcept
      requires(std::convertible_to<OtherMatrix, Matrix>)
    {
      assign(std::from_range, other_mat.columns());

      return *this;
    }

    template <class Range = std::array<typename Matrix::value_type, Matrix::row_size()>>
    constexpr basic_column_views & operator=(std::initializer_list<Range> const list) noexcept
    {
      assign(std::from_range, list);

      return *this;
    }

    template <class Iter, std::sentinel_for<Iter> Sent>
    constexpr void assign(Iter const first, Sent const last)
    {
      assign(std::ranges::subrange{first, last});
    }

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range)
    {
      for(auto [to, from] : std::views::zip(*this, std::forward<Range>(range)))
        to.assign(std::from_range, from);
    }

    [[nodiscard]]
    constexpr auto begin() const noexcept
    {
      return view.begin();
    }

    [[nodiscard]]
    constexpr auto end() const noexcept
    {
      return view.end();
    }

    [[nodiscard]]
    constexpr bool operator==(basic_column_views const & other) const noexcept
    {
      return std::ranges::equal(*this, other);
    }

    [[nodiscard]]
    constexpr bool operator!=(basic_column_views const & other) const noexcept
    {
      return not(*this == other);
    }

  private:
    struct invoke_column
    {
        basic_column_views * self = nullptr;

        constexpr auto operator()(typename Matrix::difference_type const i) const noexcept
        {
          return self->mat->column(i);
        }
    };

    Matrix * mat = nullptr;
    std::ranges::transform_view<std::ranges::iota_view<typename Matrix::difference_type, typename Matrix::difference_type>,
                                invoke_column>
      view;
};

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
class basic_row_view : public std::ranges::view_interface<basic_row_view<Iter, ColumnSize, RowSize, Layout>>
{
  public:
    template <class OtherIter>
    explicit constexpr basic_row_view(OtherIter const iter) noexcept requires(std::convertible_to<OtherIter, Iter>)
    : iter(iter)
    {
    }

    constexpr basic_row_view(basic_row_view const & other) noexcept
    : iter(other.iter)
    {
    }

    template <class OtherIter>
    constexpr basic_row_view(basic_row_view<OtherIter, ColumnSize, RowSize, Layout> const & other) noexcept
      requires(std::convertible_to<Iter, OtherIter>)
    : iter(other.iter)
    {
    }

    template <class OtherIter, std::size_t OtherColumnSize, layout OtherLayout>
    constexpr basic_row_view & operator=(
      basic_row_view<OtherIter, OtherColumnSize, RowSize, OtherLayout> const & other) noexcept
      requires(std::convertible_to<Iter, OtherIter>)
    {
      assign(std::from_range, other);

      return *this;
    }

    template <class OtherMatrix>
    constexpr basic_row_view & operator=(OtherMatrix const & mat) noexcept
      requires(std::decay_t<OtherMatrix>::row_size() == RowSize and std::decay_t<OtherMatrix>::column_size() == 1uz)
    {
      assign(std::from_range, mat.row(0));

      return *this;
    }

    constexpr basic_row_view & operator=(
      std::initializer_list<std::remove_const_t<std::iter_value_t<Iter>>> const list) noexcept
    {
      assign(std::from_range, list);

      return *this;
    }

    template <class OtherIter, std::sentinel_for<OtherIter> OtherSent>
    constexpr void assign(OtherIter const first, OtherSent const last)
    {
      std::ranges::copy_n(first, std::min(std::ranges::distance(first, last), std::ranges::distance(begin(), end())), begin());
    }

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range)
    {
      assign(std::forward<Range>(range).begin(), std::forward<Range>(range).end());
    }

    [[nodiscard]]
    constexpr auto begin() const noexcept
    {
      return iter;
    }

    [[nodiscard]]
    constexpr auto end() const noexcept
    {
      return std::ranges::next(iter, static_cast<std::iter_difference_t<Iter>>(RowSize));
    }

    [[nodiscard]]
    constexpr bool operator==(basic_row_view const & other) const noexcept
    {
      return std::ranges::equal(*this, other);
    }

    [[nodiscard]]
    constexpr bool operator!=(basic_row_view const & other) const noexcept
    {
      return not(*this == other);
    }

  private:
    iterator::strided_iterator<Iter, Layout == layout::row_major ? 1uz : ColumnSize> iter;
};

template <class Matrix>
class basic_row_views : public std::ranges::view_interface<basic_row_views<Matrix>>
{
  public:
    explicit constexpr basic_row_views(Matrix & mat) noexcept
    : mat(&mat),
      view(std::ranges::iota_view{static_cast<typename Matrix::difference_type>(0), Matrix::column_ssize()}, invoke_row{this})
    {
    }

    constexpr basic_row_views(basic_row_views const & other) noexcept
    : mat(other.mat),
      view(std::ranges::iota_view{static_cast<typename Matrix::difference_type>(0), Matrix::column_ssize()}, invoke_row{this})
    {
    }

    template <class OtherMatrix>
    constexpr basic_row_views(basic_row_views<OtherMatrix> const & other) noexcept
    : mat(other.mat),
      view(std::ranges::iota_view{static_cast<typename Matrix::difference_type>(0), Matrix::column_ssize()}, invoke_row{this})
    {
    }

    constexpr basic_row_views & operator=(basic_row_views const & other) noexcept
    {
      assign(std::from_range, other);

      return *this;
    }

    template <class OtherMatrix>
    constexpr basic_row_views & operator=(basic_row_views<OtherMatrix> const & other) noexcept
      requires(std::convertible_to<OtherMatrix, Matrix>)
    {
      assign(std::from_range, other);

      return *this;
    }

    template <class OtherMatrix>
    constexpr basic_row_views & operator=(OtherMatrix const & other_mat) noexcept
      requires(std::convertible_to<OtherMatrix, Matrix>)
    {
      assign(std::from_range, other_mat.rows());

      return *this;
    }

    template <class Range = std::array<typename Matrix::value_type, Matrix::row_size()>>
    constexpr basic_row_views & operator=(std::initializer_list<Range> const list) noexcept
    {
      assign(std::from_range, list);

      return *this;
    }

    template <class Iter, std::sentinel_for<Iter> Sent>
    constexpr void assign(Iter const first, Sent const last)
    {
      assign(std::ranges::subrange{first, last});
    }

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range)
    {
      for(auto [to, from] : std::views::zip(*this, std::forward<Range>(range)))
        to.assign(std::from_range, from);
    }

    [[nodiscard]]
    constexpr auto begin() const noexcept
    {
      return view.begin();
    }

    [[nodiscard]]
    constexpr auto end() const noexcept
    {
      return view.end();
    }

    [[nodiscard]]
    constexpr bool operator==(basic_row_views const & other) const noexcept
    {
      return std::ranges::equal(*this, other);
    }

    [[nodiscard]]
    constexpr bool operator!=(basic_row_views const & other) const noexcept
    {
      return not(*this == other);
    }

  private:
    struct invoke_row
    {
        basic_row_views * self = nullptr;

        constexpr auto operator()(typename Matrix::difference_type const i) const noexcept
        {
          return self->mat->row(i);
        }
    };

    Matrix * mat = nullptr;
    std::ranges::transform_view<std::ranges::iota_view<typename Matrix::difference_type, typename Matrix::difference_type>,
                                invoke_row>
      view;
};

template <class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout = layout::column_major>
requires(std::is_arithmetic_v<Arithmetic> and ColumnSize > 0uz and RowSize > 0uz
         and (Layout == layout::row_major or Layout == layout::column_major))
class matrix
{
  public:
    using value_type         = Arithmetic;
    using reference          = Arithmetic &;
    using const_reference    = Arithmetic const &;
    using pointer            = Arithmetic *;
    using const_pointer      = Arithmetic const *;
    using size_type          = std::size_t;
    using difference_type    = std::ptrdiff_t;
    using row_view           = basic_row_view<pointer, ColumnSize, RowSize, Layout>;
    using const_row_view     = basic_row_view<const_pointer, ColumnSize, RowSize, Layout>;
    using column_view        = basic_column_view<pointer, ColumnSize, RowSize, Layout>;
    using const_column_view  = basic_column_view<const_pointer, ColumnSize, RowSize, Layout>;
    using row_views          = basic_row_views<matrix>;
    using const_row_views    = basic_row_views<matrix const>;
    using column_views       = basic_column_views<matrix>;
    using const_column_views = basic_column_views<matrix const>;

    constexpr matrix() noexcept = default;

    template <class Iter, std::sentinel_for<Iter> Sent>
    constexpr matrix(Iter const first, Sent const last)
    {
      assign(first, last);
    }

    template <std::ranges::input_range Range>
    constexpr matrix(std::from_range_t, Range && range)
    {
      assign(std::from_range, std::forward<Range>(range));
    }

    constexpr matrix(std::initializer_list<Arithmetic> const list) noexcept
    : matrix(std::from_range, list)
    {
    }

    template <class OtherArithmetic, class Extents, class AccessorPolicy>
    constexpr matrix(std::mdspan<OtherArithmetic, Extents, std::layout_left, AccessorPolicy> const & md) noexcept
      requires(Extents::static_extent(0) == ColumnSize and Extents::static_extent(1) == RowSize)
    {
      for(typename Extents::index_type i = {}; i < md.extent(0); i++)
        for(typename Extents::index_type j = {}; j < md.extent(1); j++)
          (*this)[static_cast<difference_type>(i)][static_cast<difference_type>(j)] = md[i, j];
    }

    template <class OtherArithmetic, class Extents, class AccessorPolicy>
    constexpr matrix(std::mdspan<OtherArithmetic, Extents, std::layout_right, AccessorPolicy> const & md) noexcept
      requires(Extents::static_extent(0) == ColumnSize and Extents::static_extent(1) == RowSize)
    {
      for(typename Extents::index_type i = {}; i < md.extent(0); i++)
        for(typename Extents::index_type j = {}; j < md.extent(1); j++)
          (*this)[static_cast<difference_type>(i)][static_cast<difference_type>(j)] = md[i, j];
    }

    constexpr matrix(row_view const & view) noexcept requires(ColumnSize == 1)
    : matrix(std::from_range, view)
    {
    }

    constexpr matrix(const_row_view const & view) noexcept requires(ColumnSize == 1)
    : matrix(std::from_range, view)
    {
    }

    constexpr matrix(column_view const & view) noexcept requires(RowSize == 1)
    : matrix(std::from_range, view)
    {
    }

    constexpr matrix(const_column_view const & view) noexcept requires(RowSize == 1)
    : matrix(std::from_range, view)
    {
    }

    constexpr matrix(Arithmetic const value) noexcept requires(ColumnSize == 1 and RowSize == 1)
    : elements{value}
    {
    }

    template <class OtherArithmetic, layout OtherLayout>
    constexpr matrix(matrix<OtherArithmetic, ColumnSize, RowSize, OtherLayout> const & other) noexcept
    : matrix(std::from_range, other.rows() | std::views::join)
    {
    }

    constexpr matrix & operator=(std::initializer_list<Arithmetic> const list) noexcept
    {
      assign(std::from_range, list);

      return *this;
    }

    template <class Extents, class AccessorPolicy>
    constexpr matrix & operator=(std::mdspan<Arithmetic, Extents, std::layout_left, AccessorPolicy> const & md) noexcept
      requires(Extents::static_extent(0) == ColumnSize and Extents::static_extent(1) == RowSize)
    {
      for(typename Extents::index_type i = {}; i < md.extent(0); i++)
        for(typename Extents::index_type j = {}; j < md.extent(1); j++)
          (*this)[static_cast<difference_type>(i)][static_cast<difference_type>(j)] = md[i, j];

      return *this;
    }

    template <class Extents, class AccessorPolicy>
    constexpr matrix & operator=(std::mdspan<Arithmetic, Extents, std::layout_right, AccessorPolicy> const & md) noexcept
      requires(Extents::static_extent(0) == ColumnSize and Extents::static_extent(1) == RowSize)
    {
      for(typename Extents::index_type i = {}; i < md.extent(0); i++)
        for(typename Extents::index_type j = {}; j < md.extent(1); j++)
          (*this)[static_cast<difference_type>(i)][static_cast<difference_type>(j)] = md[i, j];

      return *this;
    }

    constexpr matrix & operator=(const_row_view const & view) noexcept requires(ColumnSize == 1)
    {
      assign(std::from_range, view);

      return *this;
    }

    constexpr matrix & operator=(const_column_view const & view) noexcept requires(RowSize == 1)
    {
      assign(std::from_range, view);

      return *this;
    }

    template <class OtherArithmetic, layout OtherLayout>
    constexpr matrix & operator=(matrix<OtherArithmetic, ColumnSize, RowSize, OtherLayout> const & other) noexcept
    {
      assign(std::from_range, other.rows() | std::views::join);

      return *this;
    }

    [[nodiscard]]
    constexpr operator std::mdspan<Arithmetic, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_right>() & noexcept
      requires(Layout == layout::row_major)
    {
      return {data()};
    }

    [[nodiscard]]
    constexpr operator std::mdspan<Arithmetic, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_left>() & noexcept
      requires(Layout == layout::column_major)
    {
      return {data()};
    }

    constexpr operator std::mdspan<Arithmetic, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_right>() && noexcept
      requires(Layout == layout::row_major)
      = delete;

    constexpr operator std::mdspan<Arithmetic, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_left>() && noexcept
      requires(Layout == layout::column_major)
      = delete;

    [[nodiscard]]
    constexpr operator std::mdspan<Arithmetic const, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_right>()
      const & noexcept requires(Layout == layout::row_major)
    {
      return {data()};
    }

    [[nodiscard]]
    constexpr operator std::mdspan<Arithmetic const, std::extents<std::size_t, ColumnSize, RowSize>, std::layout_left>()
      const & noexcept requires(Layout == layout::column_major)
    {
      return {data()};
    }

    [[nodiscard]]
    constexpr operator Arithmetic() && noexcept requires(ColumnSize == 1uz and RowSize == 1uz)
    {
      return *data();
    }

    [[nodiscard]]
    constexpr operator Arithmetic &() & noexcept requires(ColumnSize == 1uz and RowSize == 1uz)
    {
      return *data();
    }

    [[nodiscard]]
    constexpr operator Arithmetic const &() const & noexcept requires(ColumnSize == 1uz and RowSize == 1uz)
    {
      return *data();
    }

    template <class Iter, std::sentinel_for<Iter> Sent>
    constexpr void assign(Iter const first, Sent const last)
    {
      assign(std::from_range, std::ranges::subrange{first, last});
    }

    template <std::ranges::input_range Range>
    constexpr void assign(std::from_range_t, Range && range)
    {
      for(auto [to, from] : std::views::zip(rows() | std::views::join, std::forward<Range>(range)))
        to = from;
    }

    [[nodiscard]]
    static constexpr layout layout() noexcept
    {
      return Layout;
    }

    [[nodiscard]]
    constexpr pointer data() noexcept
    {
      return static_cast<pointer>(elements);
    }

    [[nodiscard]]
    constexpr const_pointer data() const noexcept
    {
      return static_cast<const_pointer>(elements);
    }

    [[nodiscard]]
    static constexpr size_type size() noexcept
    {
      return column_size() * row_size();
    }

    [[nodiscard]]
    static constexpr difference_type ssize() noexcept
    {
      return column_ssize() * row_ssize();
    }

    [[nodiscard]]
    static constexpr size_type column_size() noexcept
    {
      return ColumnSize;
    }

    [[nodiscard]]
    static constexpr size_type row_size() noexcept
    {
      return RowSize;
    }

    [[nodiscard]]
    static constexpr difference_type column_ssize() noexcept
    {
      return static_cast<difference_type>(ColumnSize);
    }

    [[nodiscard]]
    static constexpr difference_type row_ssize() noexcept
    {
      return static_cast<difference_type>(RowSize);
    }

    [[nodiscard]]
    static constexpr difference_type column_stride() noexcept
    {
      return Layout == layout::row_major ? 1 : column_ssize();
    }

    [[nodiscard]]
    static constexpr difference_type row_stride() noexcept
    {
      return Layout == layout::column_major ? 1 : row_ssize();
    }

    [[nodiscard]]
    constexpr row_views rows() noexcept
    {
      return row_views{*this};
    }

    [[nodiscard]]
    constexpr const_row_views rows() const noexcept
    {
      return const_row_views{*this};
    }

    [[nodiscard]]
    constexpr column_views columns() noexcept
    {
      return column_views{*this};
    }

    [[nodiscard]]
    constexpr const_column_views columns() const noexcept
    {
      return const_column_views{*this};
    }

    [[nodiscard]]
    constexpr row_view row(difference_type const i) noexcept
    {
      assert(0 <= i and i < column_ssize());

      static constexpr auto stride = Layout == layout::row_major ? row_ssize() : 1;
      return row_view{std::ranges::next(data(), i * stride)};
    }

    [[nodiscard]]
    constexpr const_row_view row(difference_type const i) const noexcept
    {
      assert(0 <= i and i < column_ssize());

      static constexpr auto stride = Layout == layout::row_major ? row_ssize() : 1;
      return const_row_view{std::ranges::next(data(), i * stride)};
    }

    [[nodiscard]]
    constexpr column_view column(difference_type const j) noexcept
    {
      assert(0 <= j and j < row_ssize());

      static constexpr auto stride = Layout == layout::column_major ? column_ssize() : 1;
      return column_view{std::ranges::next(data(), j * stride)};
    }

    [[nodiscard]]
    constexpr const_column_view column(difference_type const j) const noexcept
    {
      assert(0 <= j and j < row_ssize());

      static constexpr auto stride = Layout == layout::column_major ? column_ssize() : 1;
      return const_column_view{std::ranges::next(data(), j * stride)};
    }

    [[nodiscard]]
    constexpr row_view operator[](difference_type const i) noexcept
    {
      return row(i);
    }

    [[nodiscard]]
    constexpr const_row_view operator[](difference_type const i) const noexcept
    {
      return row(i);
    }

    [[nodiscard]]
    constexpr matrix operator-() noexcept
    {
      matrix result = *this;

      std::ranges::for_each_n(result.data(), result.size(), [](auto & element) { element = -element; });

      return result;
    }

  private:
    Arithmetic elements[ColumnSize * RowSize] = {};
};

template <class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr decltype(auto) decay(matrix<Arithmetic, ColumnSize, RowSize, Layout> & mat) noexcept
{
  if constexpr(ColumnSize == 1uz and RowSize == 1uz)
    return static_cast<Arithmetic &>(mat);
  else
    return mat;
}

template <class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr auto decay(matrix<Arithmetic, ColumnSize, RowSize, Layout> && mat) noexcept
{
  if constexpr(ColumnSize == 1uz and RowSize == 1uz)
    return static_cast<Arithmetic>(mat);
  else
    return mat;
}

template <class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr decltype(auto) decay(matrix<Arithmetic, ColumnSize, RowSize, Layout> const & mat) noexcept
{
  if constexpr(ColumnSize == 1uz and RowSize == 1uz)
    return static_cast<Arithmetic const &>(mat);
  else
    return mat;
}

template <class LhsArithmetic,
          class RhsArithmetic,
          std::size_t ColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr bool operator==(matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> const & lhs,
                          matrix<RhsArithmetic, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs.rows() == rhs.rows();
}

template <class LhsArithmetic,
          class RhsArithmetic,
          std::size_t ColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr bool operator!=(matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> const & lhs,
                          matrix<RhsArithmetic, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return not(lhs == rhs);
}

template <class LhsMatrix, class RhsMatrix>
[[nodiscard]]
constexpr bool operator==(basic_column_views<LhsMatrix> const & lhs, basic_column_views<RhsMatrix> const & rhs) noexcept
{
  return std::ranges::equal(lhs | std::views::join, rhs | std::views::join);
}

template <class LhsMatrix, class RhsMatrix>
[[nodiscard]]
constexpr bool operator!=(basic_column_views<LhsMatrix> const & lhs, basic_column_views<RhsMatrix> const & rhs) noexcept
{
  return not(lhs == rhs);
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
[[nodiscard]]
constexpr bool operator==(basic_column_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
                          matrix<Arithmetic, ColumnSize, 1uz, RhsLayout> const &          rhs) noexcept
{
  return lhs == rhs.column(0);
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
[[nodiscard]]
constexpr bool operator!=(basic_column_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
                          matrix<Arithmetic, ColumnSize, 1uz, RhsLayout> const &          rhs) noexcept
{
  return not(lhs == rhs);
}

template <class Arithmetic, std::size_t ColumnSize, layout LhsLayout, class Iter, std::size_t RowSize, layout RhsLayout>
[[nodiscard]]
constexpr bool operator==(matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> const &          lhs,
                          basic_column_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return rhs == lhs;
}

template <class Arithmetic, std::size_t ColumnSize, layout LhsLayout, class Iter, std::size_t RowSize, layout RhsLayout>
[[nodiscard]]
constexpr bool operator!=(matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> const &          lhs,
                          basic_column_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return not(lhs == rhs);
}

template <class LhsIter,
          class RhsIter,
          std::size_t ColumnSize,
          std::size_t LhsRowSize,
          std::size_t RhsRowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr bool operator==(basic_column_view<LhsIter, ColumnSize, LhsRowSize, LhsLayout> const & lhs,
                          basic_column_view<RhsIter, ColumnSize, RhsRowSize, RhsLayout> const & rhs) noexcept
{
  return std::ranges::equal(lhs, rhs);
}

template <class LhsIter,
          class RhsIter,
          std::size_t ColumnSize,
          std::size_t LhsRowSize,
          std::size_t RhsRowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr bool operator!=(basic_column_view<LhsIter, ColumnSize, LhsRowSize, LhsLayout> const & lhs,
                          basic_column_view<RhsIter, ColumnSize, RhsRowSize, RhsLayout> const & rhs) noexcept
{
  return not(lhs == rhs);
}

template <class LhsMatrix, class RhsMatrix>
[[nodiscard]]
constexpr bool operator==(basic_row_views<LhsMatrix> const & lhs, basic_row_views<RhsMatrix> const & rhs) noexcept
{
  return std::ranges::equal(lhs | std::views::join, rhs | std::views::join);
}

template <class LhsMatrix, class RhsMatrix>
[[nodiscard]]
constexpr bool operator!=(basic_row_views<LhsMatrix> const & lhs, basic_row_views<RhsMatrix> const & rhs) noexcept
{
  return not(lhs == rhs);
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
[[nodiscard]]
constexpr bool operator==(basic_row_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
                          matrix<Arithmetic, 1uz, RowSize, RhsLayout> const &          rhs) noexcept
{
  return lhs == rhs.row(0);
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
[[nodiscard]]
constexpr bool operator!=(basic_row_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
                          matrix<Arithmetic, 1uz, RowSize, RhsLayout> const &          rhs) noexcept
{
  return not(lhs == rhs);
}

template <class Arithmetic, std::size_t RowSize, layout LhsLayout, class Iter, std::size_t ColumnSize, layout RhsLayout>
[[nodiscard]]
constexpr bool operator==(matrix<Arithmetic, 1uz, RowSize, LhsLayout> const &          lhs,
                          basic_row_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return rhs == lhs;
}

template <class Arithmetic, std::size_t RowSize, layout LhsLayout, class Iter, std::size_t ColumnSize, layout RhsLayout>
[[nodiscard]]
constexpr bool operator!=(matrix<Arithmetic, 1uz, RowSize, LhsLayout> const &          lhs,
                          basic_row_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return not(lhs == rhs);
}

template <class LhsIter,
          class RhsIter,
          std::size_t LhsColumnSize,
          std::size_t RhsColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr bool operator==(basic_row_view<LhsIter, LhsColumnSize, RowSize, LhsLayout> const & lhs,
                          basic_row_view<RhsIter, RhsColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return std::ranges::equal(lhs, rhs);
}

template <class LhsIter,
          class RhsIter,
          std::size_t LhsColumnSize,
          std::size_t RhsColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr bool operator!=(basic_row_view<LhsIter, LhsColumnSize, RowSize, LhsLayout> const & lhs,
                          basic_row_view<RhsIter, RhsColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return not(lhs == rhs);
}

template <class LhsArithmetic,
          class RhsArithmetic,
          std::size_t ColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
constexpr matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> & operator+=(
  matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> &       lhs,
  matrix<RhsArithmetic, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs.rows() | std::views::join, rhs.rows() | std::views::join))
    to += from;

  return lhs;
}

template <class LhsArithmetic,
          class RhsArithmetic,
          std::size_t ColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
constexpr matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> operator+=(
  matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> &&      lhs,
  matrix<RhsArithmetic, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs += rhs;
}

template <class LhsIter,
          class RhsIter,
          std::size_t ColumnSize,
          std::size_t LhsRowSize,
          std::size_t RhsRowSize,
          layout      LhsLayout,
          layout      RhsLayout>
constexpr basic_column_view<LhsIter, ColumnSize, LhsRowSize, LhsLayout> operator+=(
  basic_column_view<LhsIter, ColumnSize, LhsRowSize, LhsLayout> const & lhs,
  basic_column_view<RhsIter, ColumnSize, RhsRowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs, rhs))
    to += from;

  return lhs;
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
constexpr basic_column_view<Iter, ColumnSize, RowSize, LhsLayout> operator+=(
  basic_column_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
  matrix<Arithmetic, ColumnSize, 1uz, RhsLayout> const &          rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs, rhs.row(0)))
    to += from;

  return lhs;
}

template <class Arithmetic, std::size_t ColumnSize, layout LhsLayout, class Iter, std::size_t RowSize, layout RhsLayout>
constexpr matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> & operator+=(
  matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> &                lhs,
  basic_column_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs.row(0), rhs))
    to += from;

  return lhs;
}

template <class Arithmetic, std::size_t ColumnSize, layout LhsLayout, class Iter, std::size_t RowSize, layout RhsLayout>
constexpr matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> operator+=(
  matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> &&               lhs,
  basic_column_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs += rhs;
}

template <class LhsIter,
          class RhsIter,
          std::size_t LhsColumnSize,
          std::size_t RhsColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
constexpr basic_row_view<LhsIter, LhsColumnSize, RowSize, LhsLayout> operator+=(
  basic_row_view<LhsIter, LhsColumnSize, RowSize, LhsLayout> const & lhs,
  basic_row_view<RhsIter, RhsColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs, rhs))
    to += from;

  return lhs;
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
constexpr basic_row_view<Iter, ColumnSize, RowSize, LhsLayout> operator+=(
  basic_row_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
  matrix<Arithmetic, 1uz, RowSize, RhsLayout> const &          rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs, rhs.column(0)))
    to += from;

  return lhs;
}

template <class Arithmetic, std::size_t RowSize, layout LhsLayout, class Iter, std::size_t ColumnSize, layout RhsLayout>
constexpr matrix<Arithmetic, 1uz, RowSize, LhsLayout> & operator+=(
  matrix<Arithmetic, 1uz, RowSize, LhsLayout> &                lhs,
  basic_row_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs.column(0), rhs))
    to += from;

  return lhs;
}

template <class Arithmetic, std::size_t RowSize, layout LhsLayout, class Iter, std::size_t ColumnSize, layout RhsLayout>
constexpr matrix<Arithmetic, 1uz, RowSize, LhsLayout> operator+=(
  matrix<Arithmetic, 1uz, RowSize, LhsLayout> &&               lhs,
  basic_row_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs += rhs;
}

template <class LhsArithmetic,
          class RhsArithmetic,
          std::size_t ColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator+(matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> const & lhs,
                         matrix<RhsArithmetic, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  matrix<std::common_type_t<LhsArithmetic, RhsArithmetic>, ColumnSize, RowSize, LhsLayout> result;

  for(auto [to, lhs_from, rhs_from] :
      std::views::zip(result.rows() | std::views::join, lhs.rows() | std::views::join, rhs.rows() | std::views::join))
    to = lhs_from + rhs_from;

  return result;
}

template <class LhsIter,
          class RhsIter,
          std::size_t ColumnSize,
          std::size_t LhsRowSize,
          std::size_t RhsRowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator+(basic_column_view<LhsIter, ColumnSize, LhsRowSize, LhsLayout> const & lhs,
                         basic_column_view<RhsIter, ColumnSize, RhsRowSize, RhsLayout> const & rhs) noexcept
{
  matrix<std::common_type_t<std::iter_value_t<LhsIter>, std::iter_value_t<RhsIter>>, ColumnSize, 1uz, LhsLayout> result;

  for(auto [to, lhs_from, rhs_from] : std::views::zip(result.column(0), lhs, rhs))
    to = lhs_from + rhs_from;

  return result;
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
[[nodiscard]]
constexpr auto operator+(basic_column_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
                         matrix<Arithmetic, ColumnSize, 1uz, RhsLayout> const &          rhs) noexcept
{
  return lhs + rhs.column(0);
}

template <class Arithmetic, std::size_t ColumnSize, layout LhsLayout, class Iter, std::size_t RowSize, layout RhsLayout>
[[nodiscard]]
constexpr auto operator+(matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> const &          lhs,
                         basic_column_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs.column(0) + rhs;
}

template <class LhsIter,
          class RhsIter,
          std::size_t LhsColumnSize,
          std::size_t RhsColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator+(basic_row_view<LhsIter, LhsColumnSize, RowSize, LhsLayout> const & lhs,
                         basic_row_view<RhsIter, RhsColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  matrix<std::common_type_t<std::iter_value_t<LhsIter>, std::iter_value_t<RhsIter>>, 1uz, RowSize, LhsLayout> result;

  for(auto [to, lhs_from, rhs_from] : std::views::zip(result.row(0), lhs, rhs))
    to = lhs_from + rhs_from;

  return result;
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
[[nodiscard]]
constexpr auto operator+(basic_row_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
                         matrix<Arithmetic, 1uz, RowSize, RhsLayout> const &          rhs) noexcept
{
  return lhs + rhs.row(0);
}

template <class Arithmetic, std::size_t RowSize, layout LhsLayout, class Iter, std::size_t ColumnSize, layout RhsLayout>
[[nodiscard]]
constexpr auto operator+(matrix<Arithmetic, 1uz, RowSize, LhsLayout> const &          lhs,
                         basic_row_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs.row(0) + rhs;
}

template <class LhsArithmetic,
          class RhsArithmetic,
          std::size_t ColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
constexpr matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> & operator-=(
  matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> &       lhs,
  matrix<RhsArithmetic, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs.rows() | std::views::join, rhs.rows() | std::views::join))
    to -= from;

  return lhs;
}

template <class LhsArithmetic,
          class RhsArithmetic,
          std::size_t ColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
constexpr matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> operator-=(
  matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> &&      lhs,
  matrix<RhsArithmetic, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs -= rhs;
}

template <class LhsIter,
          class RhsIter,
          std::size_t ColumnSize,
          std::size_t LhsRowSize,
          std::size_t RhsRowSize,
          layout      LhsLayout,
          layout      RhsLayout>
constexpr basic_column_view<LhsIter, ColumnSize, LhsRowSize, LhsLayout> operator-=(
  basic_column_view<LhsIter, ColumnSize, LhsRowSize, LhsLayout> const & lhs,
  basic_column_view<RhsIter, ColumnSize, RhsRowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs, rhs))
    to -= from;

  return lhs;
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
constexpr basic_column_view<Iter, ColumnSize, RowSize, LhsLayout> operator-=(
  basic_column_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
  matrix<Arithmetic, ColumnSize, 1uz, RhsLayout> const &          rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs, rhs.row(0)))
    to -= from;

  return lhs;
}

template <class Arithmetic, std::size_t ColumnSize, layout LhsLayout, class Iter, std::size_t RowSize, layout RhsLayout>
constexpr matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> & operator-=(
  matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> &                lhs,
  basic_column_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs.row(0), rhs))
    to -= from;

  return lhs;
}

template <class Arithmetic, std::size_t ColumnSize, layout LhsLayout, class Iter, std::size_t RowSize, layout RhsLayout>
constexpr matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> operator-=(
  matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> &&               lhs,
  basic_column_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs -= rhs;
}

template <class LhsIter,
          class RhsIter,
          std::size_t LhsColumnSize,
          std::size_t RhsColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
constexpr basic_row_view<LhsIter, LhsColumnSize, RowSize, LhsLayout> operator-=(
  basic_row_view<LhsIter, LhsColumnSize, RowSize, LhsLayout> const & lhs,
  basic_row_view<RhsIter, RhsColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs, rhs))
    to -= from;

  return lhs;
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
constexpr basic_row_view<Iter, ColumnSize, RowSize, LhsLayout> operator-=(
  basic_row_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
  matrix<Arithmetic, 1uz, RowSize, RhsLayout> const &          rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs, rhs.column(0)))
    to -= from;

  return lhs;
}

template <class Arithmetic, std::size_t RowSize, layout LhsLayout, class Iter, std::size_t ColumnSize, layout RhsLayout>
constexpr matrix<Arithmetic, 1uz, RowSize, LhsLayout> & operator-=(
  matrix<Arithmetic, 1uz, RowSize, LhsLayout> &                lhs,
  basic_row_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  for(auto [to, from] : std::views::zip(lhs.column(0), rhs))
    to -= from;

  return lhs;
}

template <class Arithmetic, std::size_t RowSize, layout LhsLayout, class Iter, std::size_t ColumnSize, layout RhsLayout>
constexpr matrix<Arithmetic, 1uz, RowSize, LhsLayout> operator-=(
  matrix<Arithmetic, 1uz, RowSize, LhsLayout> &&               lhs,
  basic_row_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs -= rhs;
}

template <class LhsArithmetic,
          class RhsArithmetic,
          std::size_t ColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator-(matrix<LhsArithmetic, ColumnSize, RowSize, LhsLayout> const & lhs,
                         matrix<RhsArithmetic, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  matrix<std::common_type_t<LhsArithmetic, RhsArithmetic>, ColumnSize, RowSize, LhsLayout> result;

  for(auto [to, lhs_from, rhs_from] :
      std::views::zip(result.rows() | std::views::join, lhs.rows() | std::views::join, rhs.rows() | std::views::join))
    to = lhs_from - rhs_from;

  return result;
}

template <class LhsIter,
          class RhsIter,
          std::size_t ColumnSize,
          std::size_t LhsRowSize,
          std::size_t RhsRowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator-(basic_column_view<LhsIter, ColumnSize, LhsRowSize, LhsLayout> const & lhs,
                         basic_column_view<RhsIter, ColumnSize, RhsRowSize, RhsLayout> const & rhs) noexcept
{
  matrix<std::common_type_t<std::iter_value_t<LhsIter>, std::iter_value_t<RhsIter>>, ColumnSize, 1uz, LhsLayout> result;

  for(auto [to, lhs_from, rhs_from] : std::views::zip(result.column(0), lhs, rhs))
    to = lhs_from - rhs_from;

  return result;
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
[[nodiscard]]
constexpr auto operator-(basic_column_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
                         matrix<Arithmetic, ColumnSize, 1uz, RhsLayout> const &          rhs) noexcept
{
  return lhs - rhs.column(0);
}

template <class Arithmetic, std::size_t ColumnSize, layout LhsLayout, class Iter, std::size_t RowSize, layout RhsLayout>
[[nodiscard]]
constexpr auto operator-(matrix<Arithmetic, ColumnSize, 1uz, LhsLayout> const &          lhs,
                         basic_column_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs.column(0) - rhs;
}

template <class LhsIter,
          class RhsIter,
          std::size_t LhsColumnSize,
          std::size_t RhsColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator-(basic_row_view<LhsIter, LhsColumnSize, RowSize, LhsLayout> const & lhs,
                         basic_row_view<RhsIter, RhsColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  matrix<std::common_type_t<std::iter_value_t<LhsIter>, std::iter_value_t<RhsIter>>, 1uz, RowSize, LhsLayout> result;

  for(auto [to, lhs_from, rhs_from] : std::views::zip(result.row(0), lhs, rhs))
    to = lhs_from - rhs_from;

  return result;
}

template <class Iter, std::size_t ColumnSize, std::size_t RowSize, layout LhsLayout, class Arithmetic, layout RhsLayout>
[[nodiscard]]
constexpr auto operator-(basic_row_view<Iter, ColumnSize, RowSize, LhsLayout> const & lhs,
                         matrix<Arithmetic, 1uz, RowSize, RhsLayout> const &          rhs) noexcept
{
  return lhs - rhs.row(0);
}

template <class Arithmetic, std::size_t RowSize, layout LhsLayout, class Iter, std::size_t ColumnSize, layout RhsLayout>
[[nodiscard]]
constexpr auto operator-(matrix<Arithmetic, 1uz, RowSize, LhsLayout> const &          lhs,
                         basic_row_view<Iter, ColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs.row(0) - rhs;
}

template <class LhsArithmetic,
          class RhsArithmetic,
          std::size_t M,
          std::size_t N,
          std::size_t P,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator*(matrix<LhsArithmetic, M, N, LhsLayout> const & lhs,
                         matrix<RhsArithmetic, N, P, RhsLayout> const & rhs) noexcept
{
  using value_type = std::common_type_t<LhsArithmetic, RhsArithmetic>;

  matrix<value_type, M, P, LhsLayout> result;

  using difference_type = std::common_type_t<typename std::decay_t<decltype(lhs)>::difference_type,
                                             typename std::decay_t<decltype(rhs)>::difference_type,
                                             typename decltype(result)::difference_type>;

  for(difference_type i = 0; i < static_cast<difference_type>(M); ++i)
    for(difference_type j = 0; j < static_cast<difference_type>(P); ++j)
      for(difference_type k = 0; k < static_cast<difference_type>(N); ++k)
        result[i][j] += lhs[i][k] * rhs[k][j];

  return decay(result);
}

template <class LhsIter,
          class RhsIter,
          std::size_t LhsColumnSize,
          std::size_t RhsColumnSize,
          std::size_t LhsRowSize,
          std::size_t RhsRowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator*(basic_column_view<LhsIter, LhsColumnSize, LhsRowSize, LhsLayout> const & lhs,
                         basic_row_view<RhsIter, RhsColumnSize, RhsRowSize, RhsLayout> const &    rhs) noexcept
{
  using value_type = std::common_type_t<std::ranges::range_value_t<std::decay_t<decltype(lhs)>>,
                                        std::ranges::range_value_t<std::decay_t<decltype(rhs)>>>;

  matrix<value_type, LhsColumnSize, RhsRowSize, LhsLayout> result;

  using difference_type = std::common_type_t<std::ranges::range_difference_t<std::decay_t<decltype(lhs)>>,
                                             std::ranges::range_difference_t<std::decay_t<decltype(rhs)>>,
                                             typename decltype(result)::difference_type>;

  for(difference_type i = 0; i < static_cast<difference_type>(LhsColumnSize); ++i)
    for(difference_type j = 0; j < static_cast<difference_type>(RhsRowSize); ++j)
      result[i][j] += lhs[i] * rhs[j];

  return decay(result);
}

template <class Arithmetic,
          class Iter,
          std::size_t LhsColumnSize,
          std::size_t RhsColumnSize,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator*(matrix<Arithmetic, LhsColumnSize, 1uz, LhsLayout> const &       lhs,
                         basic_row_view<Iter, RhsColumnSize, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs.column(0) * rhs;
}

template <class Iter,
          class Arithmetic,
          std::size_t ColumnSize,
          std::size_t LhsRowSize,
          std::size_t RhsRowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator*(basic_column_view<Iter, ColumnSize, LhsRowSize, LhsLayout> const & lhs,
                         matrix<Arithmetic, 1uz, RhsRowSize, RhsLayout> const &             rhs) noexcept
{
  return lhs * rhs.row(0);
}

template <class LhsIter,
          class RhsIter,
          std::size_t ColumnSize,
          std::size_t Size,
          std::size_t RowSize,
          layout      LhsLayout,
          layout      RhsLayout>
[[nodiscard]]
constexpr auto operator*(basic_row_view<RhsIter, ColumnSize, Size, LhsLayout> const & lhs,
                         basic_column_view<LhsIter, Size, RowSize, RhsLayout> const & rhs) noexcept
{
  using value_type = std::common_type_t<std::ranges::range_value_t<std::decay_t<decltype(lhs)>>,
                                        std::ranges::range_value_t<std::decay_t<decltype(rhs)>>>;

  return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), value_type{0});
}

template <class Arithmetic, class Iter, std::size_t Size, std::size_t RowSize, layout LhsLayout, layout RhsLayout>
[[nodiscard]]
constexpr auto operator*(matrix<Arithmetic, 1uz, Size, LhsLayout> const &          lhs,
                         basic_column_view<Iter, Size, RowSize, RhsLayout> const & rhs) noexcept
{
  return lhs.row(0) * rhs;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t Size, layout LhsLayout, layout RhsLayout>
[[nodiscard]]
constexpr auto operator*(basic_row_view<Iter, ColumnSize, Size, LhsLayout> const & lhs,
                         matrix<Arithmetic, Size, 1uz, RhsLayout> const &          rhs) noexcept
{
  return lhs * rhs.column(0);
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
[[nodiscard]]
constexpr auto operator*(matrix<LhsArithmetic, ColumnSize, RowSize, Layout> const & lhs, RhsArithmetic const rhs) noexcept
  requires(std::is_arithmetic_v<RhsArithmetic>)
{
  matrix<std::common_type_t<LhsArithmetic, RhsArithmetic>, ColumnSize, RowSize, Layout> result;

  for(auto [to, lhs_from] : std::views::zip(result.rows() | std::views::join, lhs.rows() | std::views::join))
    to = lhs_from * rhs;

  return result;
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
[[nodiscard]]
constexpr auto operator*(LhsArithmetic const lhs, matrix<RhsArithmetic, ColumnSize, RowSize, Layout> const & rhs) noexcept
  requires(std::is_arithmetic_v<LhsArithmetic>)
{
  return rhs * lhs;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
[[nodiscard]]
constexpr auto operator*(basic_column_view<Iter, ColumnSize, RowSize, Layout> const & lhs, Arithmetic const rhs) noexcept
  requires(std::is_arithmetic_v<Arithmetic>)
{
  matrix<std::common_type_t<std::ranges::range_value_t<std::decay_t<decltype(lhs)>>, Arithmetic>, ColumnSize, 1uz, Layout>
    result;

  for(auto [to, lhs_from] : std::views::zip(result.column(0), lhs))
    to = lhs_from * rhs;

  return result;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
[[nodiscard]]
constexpr auto operator*(Arithmetic const lhs, basic_column_view<Iter, ColumnSize, RowSize, Layout> const & rhs) noexcept
  requires(std::is_arithmetic_v<Arithmetic>)
{
  return rhs * lhs;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
[[nodiscard]]
constexpr auto operator*(basic_row_view<Iter, ColumnSize, RowSize, Layout> const & lhs, Arithmetic const rhs) noexcept
  requires(std::is_arithmetic_v<Arithmetic>)
{
  matrix<std::common_type_t<std::ranges::range_value_t<std::decay_t<decltype(lhs)>>, Arithmetic>, 1uz, RowSize, Layout> result;

  for(auto [to, lhs_from] : std::views::zip(result.row(0), lhs))
    to = lhs_from * rhs;

  return result;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
[[nodiscard]]
constexpr auto operator*(Arithmetic const lhs, basic_row_view<Iter, ColumnSize, RowSize, Layout> const & rhs) noexcept
  requires(std::is_arithmetic_v<Arithmetic>)
{
  return rhs * lhs;
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t Size, layout LhsLayout, layout RhsLayout>
constexpr matrix<LhsArithmetic, Size, Size, LhsLayout> & operator*=(
  matrix<LhsArithmetic, Size, Size, LhsLayout> &       lhs,
  matrix<RhsArithmetic, Size, Size, RhsLayout> const & rhs) noexcept
{
  return lhs = lhs * rhs;
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t Size, layout LhsLayout, layout RhsLayout>
constexpr matrix<LhsArithmetic, Size, Size, LhsLayout> operator*=(
  matrix<LhsArithmetic, Size, Size, LhsLayout> &&      lhs,
  matrix<RhsArithmetic, Size, Size, RhsLayout> const & rhs) noexcept
{
  return lhs *= rhs;
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr matrix<LhsArithmetic, ColumnSize, RowSize, Layout> & operator*=(
  matrix<LhsArithmetic, ColumnSize, RowSize, Layout> & lhs,
  RhsArithmetic const                                  rhs) noexcept requires(std::is_arithmetic_v<RhsArithmetic>)
{
  for(auto & to : lhs.rows() | std::views::join)
    to *= rhs;

  return lhs;
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr matrix<LhsArithmetic, ColumnSize, RowSize, Layout> operator*=(
  matrix<LhsArithmetic, ColumnSize, RowSize, Layout> && lhs,
  RhsArithmetic const                                   rhs) noexcept requires(std::is_arithmetic_v<RhsArithmetic>)
{
  return lhs *= rhs;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr basic_column_view<Iter, ColumnSize, RowSize, Layout> operator*=(
  basic_column_view<Iter, ColumnSize, RowSize, Layout> const & lhs,
  Arithmetic const                                             rhs) noexcept requires(std::is_arithmetic_v<Arithmetic>)
{
  for(auto & to : lhs)
    to *= rhs;

  return lhs;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr basic_row_view<Iter, ColumnSize, RowSize, Layout> operator*=(
  basic_row_view<Iter, ColumnSize, RowSize, Layout> const & lhs,
  Arithmetic const                                          rhs) noexcept requires(std::is_arithmetic_v<Arithmetic>)
{
  for(auto & to : lhs)
    to *= rhs;

  return lhs;
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr matrix<LhsArithmetic, ColumnSize, RowSize, Layout> & operator/=(
  matrix<LhsArithmetic, ColumnSize, RowSize, Layout> & lhs,
  RhsArithmetic const                                  rhs) noexcept requires(std::is_arithmetic_v<RhsArithmetic>)
{
  for(auto & to : lhs.rows() | std::views::join)
    to /= rhs;

  return lhs;
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr matrix<LhsArithmetic, ColumnSize, RowSize, Layout> operator/=(
  matrix<LhsArithmetic, ColumnSize, RowSize, Layout> && lhs,
  RhsArithmetic const                                   rhs) noexcept requires(std::is_arithmetic_v<RhsArithmetic>)
{
  return lhs /= rhs;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr basic_column_view<Iter, ColumnSize, RowSize, Layout> operator/=(
  basic_column_view<Iter, ColumnSize, RowSize, Layout> const & lhs,
  Arithmetic const                                             rhs) noexcept requires(std::is_arithmetic_v<Arithmetic>)
{
  for(auto & to : lhs)
    to /= rhs;

  return lhs;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
constexpr basic_row_view<Iter, ColumnSize, RowSize, Layout> operator/=(
  basic_row_view<Iter, ColumnSize, RowSize, Layout> const & lhs,
  Arithmetic const                                          rhs) noexcept requires(std::is_arithmetic_v<Arithmetic>)
{
  for(auto & to : lhs)
    to /= rhs;

  return lhs;
}

template <class LhsArithmetic, class RhsArithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
[[nodiscard]]
constexpr auto operator/(matrix<LhsArithmetic, ColumnSize, RowSize, Layout> const & lhs, RhsArithmetic const rhs) noexcept
  requires(std::is_arithmetic_v<RhsArithmetic>)
{
  matrix<std::common_type_t<LhsArithmetic, RhsArithmetic>, ColumnSize, RowSize, Layout> result;

  for(auto [to, lhs_from] : std::views::zip(result.rows() | std::views::join, lhs.rows() | std::views::join))
    to = lhs_from / rhs;

  return result;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
[[nodiscard]]
constexpr auto operator/(basic_column_view<Iter, ColumnSize, RowSize, Layout> const & lhs, Arithmetic const rhs) noexcept
  requires(std::is_arithmetic_v<Arithmetic>)
{
  matrix<std::common_type_t<std::ranges::range_value_t<std::decay_t<decltype(lhs)>>, Arithmetic>, ColumnSize, 1uz, Layout>
    result;

  for(auto [to, lhs_from] : std::views::zip(result.column(0), lhs))
    to = lhs_from / rhs;

  return result;
}

template <class Iter, class Arithmetic, std::size_t ColumnSize, std::size_t RowSize, layout Layout>
[[nodiscard]]
constexpr auto operator/(basic_row_view<Iter, ColumnSize, RowSize, Layout> const & lhs, Arithmetic const rhs) noexcept
  requires(std::is_arithmetic_v<Arithmetic>)
{
  matrix<std::common_type_t<std::ranges::range_value_t<std::decay_t<decltype(lhs)>>, Arithmetic>, 1uz, RowSize, Layout> result;

  for(auto [to, lhs_from] : std::views::zip(result.row(0), lhs))
    to = lhs_from / rhs;

  return result;
}
}