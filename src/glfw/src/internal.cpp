#include "internal.hpp"

glfw::library glfw::internal::init_library()
{
  return {};
}

glfw::monitor const & glfw::internal::try_emplace_monitor(GLFWmonitor * glfw_monitor)
{
  auto found = monitors.find(monitor(glfw_monitor));

  return found == monitors.end() ? *monitors.emplace_hint(monitors.end(), monitor(glfw_monitor)) : *found;
}

std::set<glfw::monitor> glfw::internal::monitors;
