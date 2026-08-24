#pragma once

#include <array>

namespace khronos
{
class graphical_device;
class graphics_pipeline;
class index_transfer_buffer;
class library;
class memory_buffer;
class present_window;
class render_window;
class staging_buffer;
class transfer_buffer;
class vertex_transfer_buffer;

struct vertex
{
    std::array<float, 2> pos;
    std::array<float, 3> colour;
};

}