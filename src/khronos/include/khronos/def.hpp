#pragma once

#include <array>

namespace khronos
{
class graphical_device;
class library;
class present_window;
class render_window;

struct vertex
{
    std::array<float, 2> pos;
    std::array<float, 3> colour;
};
}