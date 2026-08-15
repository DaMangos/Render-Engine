#pragma once

#include <glfw/def.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <ostream>

namespace khronos
{
class library
{
  public:
    library(std::ostream * const vk_verbose_out,
            std::ostream * const vk_info_out,
            std::ostream * const vk_warning_out,
            std::ostream * const vk_error_out);

    library(library &&) noexcept = default;

    library(library const &) noexcept = delete;

    library & operator=(library &&) noexcept = default;

    library & operator=(library const &) noexcept = delete;

    ~library() = default;

    [[nodiscard]]
    class present_window create_present_window(glfw::dimensions<int, 2> const & size,
                                               std::string const &              title) const;

    [[nodiscard]]
    class present_window create_present_window(glfw::dimensions<int, 2> const & size,
                                               std::string const &              title,
                                               glfw::window const &             share) const;

    [[nodiscard]]
    class present_window create_present_window(glfw::dimensions<int, 2> const & size,
                                               std::string const &              title,
                                               glfw::monitor const &            monitor) const;

    [[nodiscard]]
    class present_window create_present_window(glfw::dimensions<int, 2> const & size,
                                               std::string const &              title,
                                               glfw::window const &             share,
                                               glfw::monitor const &            monitor) const;

    [[nodiscard]]
    class graphical_device find_graphical_device(present_window const & window) const;

  private:
    std::shared_ptr<vk::raii::Context const>                context;
    std::shared_ptr<vk::raii::Instance const>               instance;
    std::shared_ptr<vk::raii::DebugUtilsMessengerEXT const> debug_utils_messenger;
};
}