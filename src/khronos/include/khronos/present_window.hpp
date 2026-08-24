// #pragma once

// #include <glfw/window.hpp>
// #include <khronos/fwd.hpp>

// #include <vulkan/vulkan_raii.hpp>

// namespace khronos
// {
// class present_window : public glfw::window
// {
//   private:
//     friend library;

//     present_window(library const & library, glfw::window && window);

//   protected:
//     handle<vk::raii::SurfaceKHR, dependences<vk::raii::Instance>> surface = nullhandle;
// };
// }