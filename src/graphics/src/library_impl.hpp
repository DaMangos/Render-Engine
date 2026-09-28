#pragma once

#include "vulakn_handles.hpp"

namespace graphics
{
struct library_impl
{
    vulkan::context                              context;
    vulkan::instance                             instance;
    std::optional<vulkan::debug_utils_messenger> debug_utils_messenger;
};
}