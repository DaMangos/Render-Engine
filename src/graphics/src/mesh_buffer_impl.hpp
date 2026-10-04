#pragma once

#include "buffer.hpp"
#include "device_memory_resource.hpp"

#include <graphics/mesh_buffer.hpp>

namespace graphics
{
struct mesh_buffer_impl
{
    struct mesh_range
    {
        std::pmr::vector<vertex>::difference_type vertices_begin;
        std::pmr::vector<vertex>::difference_type vertices_end;

        std::pmr::vector<std::uint32_t>::difference_type indices_begin;
        std::pmr::vector<std::uint32_t>::difference_type indices_end;

        std::pmr::vector<std::byte>::difference_type textures_begin;
        std::pmr::vector<std::byte>::difference_type textures_end;
    };

    std::unique_ptr<device_memory_resource<staging_buffer>> device_memory_resource;
    std::pmr::vector<vertex>                                vertex_buffer;
    std::pmr::vector<std::uint32_t>                         index_buffer;
    std::pmr::vector<std::byte>                             texture_buffer;
    std::vector<mesh_range>                                 meshes;
};
}