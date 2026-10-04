#include "buffer.hpp"
#include "device_memory_resource.hpp"
#include "graphical_device_impl.hpp"
#include "mesh_buffer_impl.hpp"

#include <graphics/graphical_device.hpp>
#include <graphics/graphics_pipeline.hpp>
#include <graphics/mesh_buffer.hpp>
#include <logging/logging.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <cstddef>
#include <cstring>
#include <ranges>

namespace
{
[[nodiscard]]
static graphics::mesh_buffer_impl create_mesh_buffer_impl(graphics::vulkan::device const & device)
{
  auto device_memory_resource = std::make_unique<graphics::device_memory_resource<graphics::staging_buffer>>(
    device.as_dependencies());

  auto * const resource_ptr = device_memory_resource.get();

  return {
    std::move(device_memory_resource),
    std::pmr::vector<graphics::vertex>{resource_ptr},
    std::pmr::vector<std::uint32_t>{resource_ptr},
    std::pmr::vector<std::byte>{resource_ptr},
    {},
  };
}
}

graphics::mesh_buffer::mesh_buffer(graphical_device const & graphical_device)
: self(std::make_unique<mesh_buffer_impl>(create_mesh_buffer_impl(graphical_device.self->device)))
{
}

graphics::mesh_buffer::~mesh_buffer() noexcept = default;

void graphics::mesh_buffer::emplace_back(std::span<vertex const> const        vertices,
                                         std::span<std::uint32_t const> const indices,
                                         std::span<std::byte const> const     texture)
{
  self->meshes.push_back({
    .vertices_begin = std::ranges::ssize(self->vertex_buffer),
    .vertices_end   = std::ranges::ssize(self->vertex_buffer) + std::ranges::ssize(vertices),
    .indices_begin  = std::ranges::ssize(self->index_buffer),
    .indices_end    = std::ranges::ssize(self->index_buffer) + std::ranges::ssize(indices),
    .textures_begin = std::ranges::ssize(self->texture_buffer),
    .textures_end   = std::ranges::ssize(self->texture_buffer) + std::ranges::ssize(texture),
  });

  self->vertex_buffer.append_range(vertices);
  self->index_buffer.append_range(indices);
  self->texture_buffer.append_range(texture);
}

void graphics::mesh_buffer::push_back(mesh const & mesh)
{
  emplace_back(mesh.vertices, mesh.indices, mesh.texture);
}

void graphics::mesh_buffer::pop_back() noexcept
{
  self->vertex_buffer.resize(static_cast<std::size_t>(self->meshes.back().vertices_begin));
  self->index_buffer.resize(static_cast<std::size_t>(self->meshes.back().indices_begin));
  self->texture_buffer.resize(static_cast<std::size_t>(self->meshes.back().textures_begin));
  self->meshes.pop_back();
}

void graphics::mesh_buffer::clear() noexcept
{
  self->meshes.clear();
  self->vertex_buffer.clear();
  self->index_buffer.clear();
  self->texture_buffer.clear();
}

void graphics::mesh_buffer::reserve_vertices(std::size_t const size)
{
  self->vertex_buffer.reserve(size);
}

void graphics::mesh_buffer::reserve_indices(std::size_t const size)
{
  self->index_buffer.reserve(size);
}

void graphics::mesh_buffer::reserve_texture(std::size_t const size)
{
  self->texture_buffer.reserve(size);
}

graphics::mesh_buffer::iterator graphics::mesh_buffer::begin()
{
  return {*this, 0};
}

graphics::mesh_buffer::const_iterator graphics::mesh_buffer::cbegin() const
{
  return {*this, 0};
}

graphics::mesh_buffer::iterator graphics::mesh_buffer::end()
{
  return {*this, std::ranges::ssize(self->meshes)};
}

graphics::mesh_buffer::const_iterator graphics::mesh_buffer::cend() const
{
  return {*this, std::ranges::ssize(self->meshes)};
}

std::span<graphics::vertex> graphics::mesh_buffer::vertices()
{
  return self->vertex_buffer;
}

std::span<graphics::vertex const> graphics::mesh_buffer::vertices() const noexcept
{
  return self->vertex_buffer;
}

std::span<graphics::vertex> graphics::mesh_buffer::vertices(std::size_t const mesh_index)
{
  return {
    std::ranges::next(self->vertex_buffer.begin(), self->meshes[mesh_index].vertices_begin),
    std::ranges::next(self->vertex_buffer.begin(), self->meshes[mesh_index].vertices_end),
  };
}

std::span<graphics::vertex const> graphics::mesh_buffer::vertices(std::size_t const mesh_index) const noexcept
{
  return {
    std::ranges::next(self->vertex_buffer.begin(), self->meshes[mesh_index].vertices_begin),
    std::ranges::next(self->vertex_buffer.begin(), self->meshes[mesh_index].vertices_end),
  };
}

std::span<std::uint32_t> graphics::mesh_buffer::indices() noexcept
{
  return self->index_buffer;
}

std::span<std::uint32_t const> graphics::mesh_buffer::indices() const noexcept
{
  return self->index_buffer;
}

std::span<std::uint32_t> graphics::mesh_buffer::indices(std::size_t const mesh_index) noexcept
{
  return {
    std::ranges::next(self->index_buffer.begin(), self->meshes[mesh_index].indices_begin),
    std::ranges::next(self->index_buffer.begin(), self->meshes[mesh_index].indices_end),
  };
}

std::span<std::uint32_t const> graphics::mesh_buffer::indices(std::size_t const mesh_index) const noexcept
{
  return {
    std::ranges::next(self->index_buffer.begin(), self->meshes[mesh_index].indices_begin),
    std::ranges::next(self->index_buffer.begin(), self->meshes[mesh_index].indices_end),
  };
}

std::span<std::byte> graphics::mesh_buffer::texture() noexcept
{
  return self->texture_buffer;
}

std::span<std::byte const> graphics::mesh_buffer::texture() const noexcept
{
  return self->texture_buffer;
}

std::span<std::byte> graphics::mesh_buffer::texture(std::size_t const mesh_index) noexcept
{
  return {
    std::ranges::next(self->texture_buffer.begin(), self->meshes[mesh_index].textures_begin),
    std::ranges::next(self->texture_buffer.begin(), self->meshes[mesh_index].textures_end),
  };
}

std::span<std::byte const> graphics::mesh_buffer::texture(std::size_t const mesh_index) const noexcept
{
  return {
    std::ranges::next(self->texture_buffer.begin(), self->meshes[mesh_index].textures_begin),
    std::ranges::next(self->texture_buffer.begin(), self->meshes[mesh_index].textures_end),
  };
}
