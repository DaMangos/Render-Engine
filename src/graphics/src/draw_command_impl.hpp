#pragma once

#include "vulakn_handles.hpp"

namespace graphics
{
struct draw_command_impl
{
    vulkan::fences          draw_fences;
    vulkan::semaphores      render_complete_semaphores;
    vulkan::semaphores      present_complete_semaphores;
    vulkan::command_pool    command_pool;
    vulkan::command_buffers command_buffers;
};
}