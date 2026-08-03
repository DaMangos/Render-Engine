#pragma once

#include <memory>

namespace graphics::detail::glfw
{
[[nodiscard]]
std::shared_ptr<void> init_library();
}