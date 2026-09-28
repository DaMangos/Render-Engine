#pragma once

#include "device_memory_resource.hpp"
#include "vulakn_handles.hpp"
#include "vulkan/vulkan.hpp"

#include <graphics/mesh_buffer.hpp>

#include <optional>
#include <unordered_map>

namespace graphics
{
struct mesh_buffer_impl
{
    std::unique_ptr<device_memory_resource> resource;
    std::pmr::vector<mesh_buffer::vertex>   vertex_buffer;
    std::pmr::vector<std::uint32_t>         index_buffer;
    std::pmr::vector<std::byte>             texture_buffer;

    struct mesh_range
    {
        std::uint32_t id;

        std::pmr::vector<mesh_buffer::vertex>::size_type vertices_begin;
        std::pmr::vector<mesh_buffer::vertex>::size_type vertices_end;

        std::pmr::vector<std::uint32_t>::size_type indices_begin;
        std::pmr::vector<std::uint32_t>::size_type indices_end;

        std::pmr::vector<std::byte>::size_type textures_begin;
        std::pmr::vector<std::byte>::size_type textures_end;
    };

    std::vector<mesh_range>                                               meshes;
    std::unordered_map<std::uint32_t, std::vector<mesh_range>::size_type> id_to_mesh;

    std::optional<vulkan::fence>         buffer_in_use;
    std::optional<vulkan::buffer>        device_local_buffer;
    std::optional<vulkan::device_memory> device_local_device_memory;
    std::optional<vk::DeviceSize>        buffer_sizes;

    vulkan::fence          transfer_complete;
    vulkan::command_pool   transfer_command_pool;
    vulkan::command_buffer transfer_command_buffer;
};
}