#include <glfw/def.hpp>
#include <GLFW/glfw3.h>
#include <glfw/library.hpp>
#include <glfw/monitor.hpp>

#include <cassert>
#include <cstddef>

std::strong_ordering glfw::monitor::operator<=>(monitor const & other) const noexcept
{
  return ptr <=> other.ptr;
}

glfw::int2 glfw::monitor::get_pos() const
{
  int2 pos = {};

  glfwGetMonitorPos(ptr, &pos.x, &pos.y);

  return pos;
}

glfw::workarea glfw::monitor::get_workarea() const
{
  workarea wa;

  glfwGetMonitorWorkarea(ptr, &wa.pos.x, &wa.pos.y, &wa.pos.width, &wa.pos.height);

  return wa;
}

glfw::int2 glfw::monitor::get_physical_size_millimeters() const
{
  int2 size = {};

  glfwGetMonitorPhysicalSize(ptr, &size.width, &size.height);

  return size;
}

glfw::float2 glfw::monitor::get_content_scale() const
{
  float2 scale = {};

  glfwGetMonitorContentScale(ptr, &scale.x, &scale.y);

  return scale;
}

std::string_view glfw::monitor::get_name() const
{
  return glfwGetMonitorName(ptr);
}

std::vector<glfw::vidmode> glfw::monitor::get_video_modes() const
{
  int count = 0;

  GLFWvidmode const * const glfw_vidmodes = glfwGetVideoModes(ptr, &count);

  assert(glfw_vidmodes);

  std::vector<vidmode> vidmodes;

  vidmodes.reserve(static_cast<std::size_t>(count));

  for(auto const & glfw_vidmode : std::span{glfw_vidmodes, static_cast<std::size_t>(count)})
    vidmodes.push_back({
      .size         = {.height = glfw_vidmode.height, .width = glfw_vidmode.width},
      .bits         = {.red = glfw_vidmode.redBits, .blue = glfw_vidmode.blueBits, .green = glfw_vidmode.greenBits},
      .refresh_rate = glfw_vidmode.refreshRate,
    });

  return vidmodes;
}

glfw::vidmode glfw::monitor::get_current_video_mode() const
{
  GLFWvidmode const * const glfw_vidmode = glfwGetVideoMode(ptr);

  assert(glfw_vidmode);

  return {
    .size         = {.height = glfw_vidmode->height, .width = glfw_vidmode->width},
    .bits         = {.red = glfw_vidmode->redBits, .blue = glfw_vidmode->blueBits, .green = glfw_vidmode->greenBits},
    .refresh_rate = glfw_vidmode->refreshRate,
  };
}

glfw::monitor::monitor(GLFWmonitor * ptr) noexcept
: ptr(ptr)
{
}
