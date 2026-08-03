#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <memory>
#include <ostream>
#include <set>

namespace graphics::detail::vulkan
{
struct init_library_retrun_type
{
    std::shared_ptr<vk::raii::Context const>                context;
    std::shared_ptr<vk::raii::Instance const>               instance;
    std::shared_ptr<vk::raii::DebugUtilsMessengerEXT const> debug_utils_messenger;
};

[[nodiscard]]
init_library_retrun_type init_library(std::set<std::string> instance_extensions,
                                      std::set<std::string> instance_layers,
                                      std::ostream * const  vk_verbose_out,
                                      std::ostream * const  vk_info_out,
                                      std::ostream * const  vk_warning_out,
                                      std::ostream * const  vk_error_out);
}