#pragma once

#include <glfw/fwd.hpp>
#include <khronos/fwd.hpp>

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

  private:
    friend graphical_device;

    std::unique_ptr<struct library_impl, void (*)(library_impl *)> ptr;
};
}