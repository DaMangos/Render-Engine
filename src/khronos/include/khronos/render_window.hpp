// #pragma once

// #include <khronos/fwd.hpp>
// #include <khronos/present_window.hpp>

// #include <vulkan/vulkan_raii.hpp>

// #include <list>
// #include <memory>
// #include <vector>

// namespace khronos
// {
// class render_window : public present_window
// {
//   private:
//     virtual void when_framebuffer_resized(glfw::dimensions<int, 2> const &) override;

//     friend graphical_device;

//     render_window(graphical_device const & graphical_device, present_window && present_window);

//     handle<vk::raii::Device, dependences<vk::raii::PhysicalDevice>> device = nullhandle;

//     vk::SwapchainCreateInfoKHR swapchain_create_info;

//     handle<vk::raii::SwapchainKHR, dependences<vk::raii::Device, vk::raii::SurfaceKHR, vk::raii::Queue>>
//       swapchain = nullhandle;

//     struct image
//     {
//         vk::Image                                  image;
//         std::shared_ptr<vk::raii::ImageView const> image_view;
//         std::shared_ptr<vk::raii::Semaphore const> render_complete_semaphores;
//     };

//     std::vector<image> images;

//     handle<vk::raii::CommandPool, dependences<vk::raii::Device>> command_pool = nullhandle;

//     struct frame
//     {
//         std::shared_ptr<vk::raii::CommandBuffer const> command_buffer;
//         std::shared_ptr<vk::raii::Fence const>         in_flight_fence;
//         std::shared_ptr<vk::raii::Semaphore const>     present_complete_semaphores;
//     };

//     std::list<frame> frames;
// };
// }