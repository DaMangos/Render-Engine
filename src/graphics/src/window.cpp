#include "detail/window_impl.hpp"

#include <graphics/window.hpp>

#include <memory>
#include <stdexcept>

namespace
{
template <class... Args>
[[nodiscard]]
static std::unique_ptr<void, void (*)(void *)> make_window_impl(Args &&... args)
{
  return {new graphics::detail::window_impl(std::forward<Args>(args)...),
          [](void * ptr) { delete static_cast<graphics::detail::window_impl *>(ptr); }};
}

[[nodiscard]]
static graphics::detail::window_impl & get(std::unique_ptr<void, void (*)(void *)> & impl)
{
  return impl ? *static_cast<graphics::detail::window_impl *>(impl.get()) : throw std::logic_error("move after free");
}

[[nodiscard]]
static graphics::detail::window_impl const & get(std::unique_ptr<void, void (*)(void *)> const & impl)
{
  return impl ? *static_cast<graphics::detail::window_impl const *>(impl.get()) : throw std::logic_error("move after free");
}
}

graphics::window::window(pixel const          width,
                         pixel const          hight,
                         std::string const &  title,
                         std::ostream * const vk_verbose_out,
                         std::ostream * const vk_info_out,
                         std::ostream * const vk_warning_out,
                         std::ostream * const vk_error_out)
: window_impl(make_window_impl(width, hight, title, vk_verbose_out, vk_info_out, vk_warning_out, vk_error_out))
{
}

bool graphics::window::is_valueless_after_move() const noexcept
{
  return static_cast<bool>(window_impl);
}

bool graphics::window::should_close() const
{
  return get(window_impl).should_close();
}

void graphics::window::poll_events() const
{
  get(window_impl).poll_events();
}

void graphics::window::close() noexcept
{
  get(window_impl).close();
}