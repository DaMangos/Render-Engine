#pragma once

#include "buffer.hpp"
#include "vulakn_handles.hpp"

namespace graphics
{
struct graphics_pipeline_impl
{
    vulkan::pipeline_layout pipeline_layout;
    vulkan::pipeline        pipeline;

    vulkan::command_pool command_pool;

    std::list<transfer_buffer> transfer_buffers;
    vulkan::fences             transfer_complete_fences;
    vulkan::command_buffers    transfer_command_buffers;

    vulkan::semaphores      render_complete_semaphores;
    vulkan::semaphores      present_complete_semaphores;
    vulkan::fences          draw_complete_fences;
    vulkan::command_buffers draw_command_buffers;
};
}