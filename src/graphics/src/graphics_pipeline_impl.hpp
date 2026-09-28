#pragma once

#include "vulakn_handles.hpp"

namespace graphics
{
struct graphics_pipeline_impl
{
    vulkan::pipeline_layout pipeline_layout;
    vulkan::pipeline        pipeline;
};
}