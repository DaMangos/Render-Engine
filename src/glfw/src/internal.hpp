#pragma once

#include <glfw/library.hpp>
#include <glfw/monitor.hpp>

#include <set>

namespace glfw
{
class internal
{
  public:
    [[nodiscard]]
    static library init_library();

    static monitor const & try_emplace_monitor(GLFWmonitor * glfw_monitor);

    static std::set<monitor> monitors;
};
}
