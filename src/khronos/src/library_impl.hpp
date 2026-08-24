#pragma once

#include "dependencies.hpp"

#include <vulkan/vulkan_raii.hpp>

#include <ostream>

namespace khronos
{
struct user_data
{
    std::ostream * vk_verbose_out;
    std::ostream * vk_info_out;
    std::ostream * vk_warning_out;
    std::ostream * vk_error_out;
};

using context               = dependent<vk::raii::Context>;
using instance              = dependent<vk::raii::Instance, dependencies<vk::raii::Context, user_data>>;
using debug_utils_messenger = dependent<vk::raii::DebugUtilsMessengerEXT, instance::as_dependencies_type>;

struct library_impl
{
    context                              context;
    instance                             instance;
    std::optional<debug_utils_messenger> debug_utils_messenger;
};
}
