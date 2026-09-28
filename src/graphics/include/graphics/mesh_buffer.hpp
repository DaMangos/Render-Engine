#pragma once

#include <maths/matrix.hpp>

#include <cstddef>
#include <memory>
#include <vector>

namespace graphics
{
class mesh_buffer
{
  public:
    struct vertex
    {
        alignas(16) maths::column_major::float3 pos;
        alignas(16) maths::column_major::float3 uv;
    };

    struct mesh
    {
        std::uint32_t              id;
        std::vector<vertex>        vertices;
        std::vector<std::uint32_t> indices;
        std::vector<std::byte>     texture;
    };

    struct mesh_view
    {
        std::uint32_t            id;
        std::span<vertex>        vertices;
        std::span<std::uint32_t> indices;
        std::span<std::byte>     texture;
    };

    struct const_mesh_view
    {
        std::uint32_t                  id;
        std::span<vertex const>        vertices;
        std::span<std::uint32_t const> indices;
        std::span<std::byte const>     texture;
    };

    mesh_buffer(class graphical_device const & graphical_device);

    mesh_buffer(mesh_buffer &&) noexcept = default;

    mesh_buffer(mesh_buffer const &) noexcept = delete;

    mesh_buffer & operator=(mesh_buffer const &) noexcept = delete;

    mesh_buffer & operator=(mesh_buffer &&) noexcept = default;

    ~mesh_buffer() noexcept;

    void push_back(mesh const & mesh);

    void pop_back() noexcept;

    void clear() noexcept;

    std::size_t size() const noexcept;

    std::size_t byte_size() const noexcept;

    bool empty() const noexcept;

    void flush();

    void reserve_vertices(std::size_t const size);

    void reserve_indices(std::size_t const size);

    void reserve_texture(std::size_t const size);

    mesh_view operator[](std::size_t const index) noexcept;

    const_mesh_view operator[](std::size_t const index) const noexcept;

    mesh_view at(std::size_t const index);

    const_mesh_view at(std::size_t const index) const;

    mesh_view find(std::uint32_t const id);

    const_mesh_view find(std::uint32_t const id) const;

  private:
    friend class transfer_command;

    std::unique_ptr<struct mesh_buffer_impl> self;
};
}