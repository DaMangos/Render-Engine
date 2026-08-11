#include "internal.hpp"

glfw_3::library glfw_3::internal::init_library()
{
  return {};
}

glfw_3::monitor const & glfw_3::internal::try_emplace_monitor(GLFWmonitor * glfw_monitor)
{
  auto found = monitors.find(monitor(glfw_monitor));

  return found == monitors.end() ? *monitors.emplace_hint(monitors.end(), monitor(glfw_monitor)) : *found;
}

std::set<glfw_3::monitor> glfw_3::internal::monitors;
