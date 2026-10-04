#pragma once

#include "vulakn_handles.hpp"

namespace graphics
{
struct draw_command_impl
{
    vulkan::semaphores      render_complete_semaphores;
    vulkan::semaphores      present_complete_semaphores;
    vulkan::fences          present_complete_fences;
    vulkan::command_pool    draw_command_pool;
    vulkan::command_buffers draw_command_buffers;
};
}