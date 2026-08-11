#pragma once

#include <glfw_3/library.hpp>
#include <glfw_3/monitor.hpp>

#include <set>

namespace glfw_3
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
