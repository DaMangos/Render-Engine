#pragma once

#include <algorithm>
#include <array>
#include <mdspan>
#include <type_traits>

namespace maths
{
template <class Arithmetic, std::size_t M, std::size_t N>
requires(std::is_arithmetic_v<Arithmetic> and M > 0 and N > 0)
class matrix : public std::array<std::array<Arithmetic, N>, M>
{
  public:
    explicit constexpr operator std::mdspan<Arithmetic, std::extents<std::size_t, M, N>>() noexcept
    {
      return {&(*this)[0][0]};
    }

    explicit constexpr operator std::mdspan<Arithmetic const, std::extents<std::size_t, M, N>>()
      const noexcept
    {
      return {&(*this)[0][0]};
    }

    explicit constexpr operator std::span<Arithmetic, M * N>() noexcept
    {
      return {&(*this)[0][0]};
    }

    explicit constexpr operator std::span<Arithmetic const, M * N>() const noexcept
    {
      return {&(*this)[0][0]};
    }
};

template <class Arithmetic, std::size_t M, std::size_t N>
constexpr matrix<Arithmetic, M, N> operator+(matrix<Arithmetic, M, N> const & lhs,
                                             matrix<Arithmetic, M, N>         rhs) noexcept
{
  std::transform(std::span{lhs}.begin(), std::span{lhs}.end(), std::span{rhs}.begin(), std::plus{});
  return rhs;
}

template <class Arithmetic, std::size_t M, std::size_t N>
constexpr matrix<Arithmetic, M, N> operator-(matrix<Arithmetic, M, N> const & lhs,
                                             matrix<Arithmetic, M, N>         rhs) noexcept
{
  std::transform(std::span{lhs}.begin(), std::span{lhs}.end(), std::span{rhs}.begin(), std::minus{});
  return rhs;
}

template <class Arithmetic, std::size_t M, std::size_t N, std::size_t P>
constexpr matrix<Arithmetic, M, P> operator*(matrix<Arithmetic, M, N> const & lhs,
                                             matrix<Arithmetic, N, P> const & rhs) noexcept
{
  matrix<Arithmetic, M, P> result;
  for(std::size_t i = 0; i < M; ++i)
    for(std::size_t j = 0; j < P; ++j)
      for(std::size_t k = 0; k < N; ++k)
        result[i][j] += lhs[i][k] * rhs[k][j];
  return result;
}

template <class Arithmetic, std::size_t M, std::size_t N>
constexpr matrix<Arithmetic, M, N> operator*(Arithmetic const lhs, matrix<Arithmetic, M, N> rhs) noexcept
{
  std::transform(std::span{rhs}.begin(),
                 std::span{rhs}.end(),
                 std::span{rhs}.begin(),
                 [=](Arithmetic value) { return lhs * value; });
  return rhs;
}

template <class Arithmetic, std::size_t M, std::size_t N>
constexpr matrix<Arithmetic, M, N> operator*(matrix<Arithmetic, M, N> lhs, Arithmetic const rhs) noexcept
{
  std::transform(std::span{lhs}.begin(),
                 std::span{lhs}.end(),
                 std::span{lhs}.begin(),
                 [=](Arithmetic value) { return value * rhs; });
  return lhs;
}

template <class Arithmetic, std::size_t M, std::size_t N>
constexpr matrix<Arithmetic, M, N> operator/(matrix<Arithmetic, M, N> lhs, Arithmetic const rhs) noexcept
{
  std::transform(std::span{lhs}.begin(),
                 std::span{lhs}.end(),
                 std::span{lhs}.begin(),
                 [=](Arithmetic value) { return value / rhs; });
  return lhs;
}

template <class Arithmetic, std::size_t M, std::size_t N>
constexpr void row_echelon(matrix<Arithmetic, M, N> & mat) noexcept
{
  for(std::size_t k = 0; k < M and k < N; ++k)
  {
    if(mat[k][k] == Arithmetic{})
      for(std::size_t i = k + 1; i < M; ++i)
        if(mat[i][k] != Arithmetic{})
        {
          swap_rows(k, i);
          break;
        }

    if(mat[k][k] == Arithmetic{})
      continue;

    for(std::size_t i = k; i < M; ++i)
    {
      mat[i][k] = Arithmetic{};
      for(std::size_t j = k + 1; j < N; ++j)
        mat[i][j] = mat[i][j] * mat[k][k] - mat[k][j] * mat[i][k];
    }
  }
}
}