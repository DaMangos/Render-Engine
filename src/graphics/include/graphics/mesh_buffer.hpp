#pragma once

#include <graphics/detail/mesh_buffer_iterator.hpp>
#include <maths/matrix.hpp>

#include <cstddef>
#include <memory>
#include <ranges>
#include <span>
#include <vector>

namespace graphics
{
struct vertex
{
    alignas(16) maths::column_major::float3 pos;
    alignas(16) maths::column_major::float3 uv;
};

struct mesh_view
{
    std::span<vertex>        vertices;
    std::span<std::uint32_t> indices;
    std::span<std::byte>     texture;
};

struct const_mesh_view
{
    std::span<vertex const>        vertices;
    std::span<std::uint32_t const> indices;
    std::span<std::byte const>     texture;
};

struct mesh
{
    std::vector<vertex>        vertices;
    std::vector<std::uint32_t> indices;
    std::vector<std::byte>     texture;

    [[nodiscard]]
    constexpr operator mesh_view() & noexcept
    {
      return {vertices, indices, texture};
    }

    constexpr operator mesh_view() && noexcept = delete;

    [[nodiscard]]
    constexpr operator const_mesh_view() const & noexcept
    {
      return {vertices, indices, texture};
    }

    constexpr operator const_mesh_view() && noexcept = delete;
};

class mesh_buffer : std::ranges::view_interface<mesh_buffer>
{
  public:
    using value_type     = mesh;
    using iterator       = detail::mesh_buffer_iterator<false>;
    using const_iterator = detail::mesh_buffer_iterator<true>;

    mesh_buffer(class graphical_device const & graphical_device);

    mesh_buffer(mesh_buffer &&) noexcept = default;

    mesh_buffer(mesh_buffer const &) noexcept = delete;

    mesh_buffer & operator=(mesh_buffer const &) noexcept = delete;

    mesh_buffer & operator=(mesh_buffer &&) noexcept = default;

    ~mesh_buffer() noexcept;

    void emplace_back(std::span<vertex const> const        vertices,
                      std::span<std::uint32_t const> const indices,
                      std::span<std::byte const> const     texture);

    void push_back(mesh const & mesh);

    void pop_back() noexcept;

    void clear() noexcept;

    void reserve_vertices(std::size_t const size);

    void reserve_indices(std::size_t const size);

    void reserve_texture(std::size_t const size);

    [[nodiscard]]
    iterator begin();

    [[nodiscard]]
    const_iterator cbegin() const;

    [[nodiscard]]
    iterator end();

    [[nodiscard]]
    const_iterator cend() const;

    [[nodiscard]]
    std::span<vertex> vertices();

    [[nodiscard]]
    std::span<vertex const> vertices() const noexcept;

    [[nodiscard]]
    std::span<vertex> vertices(std::size_t const mesh_index);

    [[nodiscard]]
    std::span<vertex const> vertices(std::size_t const mesh_index) const noexcept;

    [[nodiscard]]
    std::span<std::uint32_t> indices() noexcept;

    [[nodiscard]]
    std::span<std::uint32_t const> indices() const noexcept;

    [[nodiscard]]
    std::span<std::uint32_t> indices(std::size_t const mesh_index) noexcept;

    [[nodiscard]]
    std::span<std::uint32_t const> indices(std::size_t const mesh_index) const noexcept;

    [[nodiscard]]
    std::span<std::byte> texture() noexcept;

    [[nodiscard]]
    std::span<std::byte const> texture() const noexcept;

    [[nodiscard]]
    std::span<std::byte> texture(std::size_t const mesh_index) noexcept;

    [[nodiscard]]
    std::span<std::byte const> texture(std::size_t const mesh_index) const noexcept;

  private:
    friend class graphics_pipeline;

    std::unique_ptr<struct mesh_buffer_impl> self;
};
}