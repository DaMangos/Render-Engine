#pragma once

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

    library & operator=(library const &) noexcept = delete;

    library & operator=(library &&) noexcept = default;

    ~library() noexcept;

  private:
    friend class present_window;

    std::unique_ptr<struct library_impl> self;
};
}