
// #include "khronos/handle.hpp"

// #include <glfw/fwd.hpp>
// #include <glfw/library.hpp>
// #include <glfw/window.hpp>
// #include <khronos/fwd.hpp>
// #include <khronos/render_window.hpp>
// #include <logging/logging.hpp>

// #include <vulkan/vulkan.hpp>
// #include <vulkan/vulkan_raii.hpp>
// #include <vulkan/vulkan_to_string.hpp>

// #include <algorithm>
// #include <cassert>
// #include <cstring>
// #include <memory>
// #include <ranges>
// #include <stdexcept>

// void khronos::render_window::when_framebuffer_resized(glfw::dimensions<int, 2> const & size)
// {
//   auto const image_extent = vk::Extent2D{}
//                               .setWidth(static_cast<std::uint32_t>(size.width))
//                               .setHeight(static_cast<std::uint32_t>(size.height));

//   logging::verbose() << "image width = " << image_extent.width << " and image height = " << image_extent.height;

//   swapchain_create_info.setImageExtent(image_extent);

//   if(swapchain)
//     swapchain_create_info.setOldSwapchain(*swapchain);

//   auto const
//     swapchain_dependences = dependency_builder<vk::raii::SwapchainKHR, vk::raii::Device, vk::raii::SurfaceKHR>{}
//                               .add_dependency(device)
//                               .add_dependency(surface)
//                               .add_dependency([
//                               ](vk::raii::SwapchainKHR const &, vk::raii::Device const &, vk::raii::SurfaceKHR const
//                               &))

//                                 swapchain.reset(swapchain_dependences, *device, *swapchain_create_info);

//   detail::emplace_data(swapchain, device);
//   detail::emplace_data(swapchain, surface);
//   detail::emplace_function(swapchain, [=](auto const &) { graphics_and_present_queue->waitIdle(); });

//   images.clear();

//   for(auto const & image : swapchain->getImages())
//   {
//     constexpr auto image_subresource_range = vk::ImageSubresourceRange{}
//                                                .setAspectMask(vk::ImageAspectFlagBits::eColor)
//                                                .setBaseMipLevel(0)
//                                                .setLevelCount(1)
//                                                .setBaseArrayLayer(0)
//                                                .setLayerCount(1);

//     auto const image_view_create_info = vk::ImageViewCreateInfo{}
//                                           .setViewType(vk::ImageViewType::e2D)
//                                           .setFormat(swapchain_create_info.imageFormat)
//                                           .setSubresourceRange(image_subresource_range)
//                                           .setImage(image);

//     auto const image_view = detail::make_shared_with_data<vk::raii::ImageView const>(*device,
//     image_view_create_info);

//     detail::emplace_data(image_view, swapchain);

//     auto const render_complete_semaphores = detail::make_shared_with_data<vk::raii::Semaphore const>(
//       *device,
//       vk::SemaphoreCreateInfo{});

//     detail::emplace_data(render_complete_semaphores, device);
//     detail::emplace_data(graphics_and_present_queue, render_complete_semaphores);

//     images.emplace_back(image, image_view, render_complete_semaphores);
//   }
// }

// khronos::render_window::render_window(graphical_device const & graphical_device, present_window && present_window)
// : present_window(std::move(window))
// {
//   using namespace std::literals;

//   auto const min_image_count = surface_capabilities.maxImageCount == 0
//                                ? std::max(3u, surface_capabilities.minImageCount)
//                                : std::clamp(3u, surface_capabilities.minImageCount,
//                                surface_capabilities.maxImageCount);

//   swapchain_create_info = vk::SwapchainCreateInfoKHR{}
//                             .setSurface(*surface)
//                             .setMinImageCount(min_image_count)
//                             .setImageArrayLayers(1)
//                             .setImageSharingMode(vk::SharingMode::eExclusive)
//                             .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
//                             .setPreTransform(surface_capabilities.currentTransform)
//                             .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
//                             .setPresentMode(vk::PresentModeKHR::eFifo)
//                             .setImageFormat(vk::Format::eB8G8R8A8Srgb)
//                             .setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear)
//                             .setClipped(vk::True);

//   render_window::when_framebuffer_resized(*this, glfw::window::get_framebuffer_size());

//   auto const command_pool_create_info = vk::CommandPoolCreateInfo{}
//                                           .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
//                                           .setQueueFamilyIndex(graphics_and_present_queue_family_index);

//   command_pool = detail::make_shared_with_data<vk::raii::CommandPool const>(*device, command_pool_create_info);

//   detail::emplace_data(command_pool, device);

//   auto const command_buffer_allocate_info = vk::CommandBufferAllocateInfo{}
//                                               .setCommandPool(*command_pool)
//                                               .setLevel(vk::CommandBufferLevel::ePrimary)
//                                               .setCommandBufferCount(2);

//   auto const create_shared_command_buffer = [&](vk::raii::CommandBuffer & command_buffer)
//   {
//     auto const shared_command_buffer = detail::make_shared_with_data<vk::raii::CommandBuffer const>(
//       std::move(command_buffer));

//     detail::emplace_data(shared_command_buffer, command_pool);

//     return shared_command_buffer;
//   };

//   for(auto && command_buffer : vk::raii::CommandBuffers(*device, command_buffer_allocate_info)
//                                  | std::views::transform(create_shared_command_buffer))
//   {
//     auto const in_flight_fence_create_info = vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled);

//     auto const in_flight_fence = detail::make_shared_with_data<vk::raii::Fence const>(*device,
//                                                                                       in_flight_fence_create_info);

//     detail::emplace_data(in_flight_fence, device);
//     detail::emplace_data(graphics_and_present_queue, in_flight_fence);

//     auto const wait_for_in_flight_fence = [=](vk::raii::CommandBuffer const &)
//     {
//       auto const wait_for_fences_result = device->waitForFences(**in_flight_fence,
//                                                                 vk::True,
//                                                                 std::numeric_limits<std::uint64_t>::max());

//       if(wait_for_fences_result < vk::Result::eSuccess)
//         logging::error() << "wait for fences returned a error: " << vk::to_string(wait_for_fences_result);

//       if(wait_for_fences_result > vk::Result::eSuccess)
//         logging::warning() << "wait for fences returned a warring: " << vk::to_string(wait_for_fences_result);
//     };

//     detail::emplace_function(command_buffer, wait_for_in_flight_fence);

//     auto const present_complete_semaphores = detail::make_shared_with_data<vk::raii::Semaphore const>(
//       *device,
//       vk::SemaphoreCreateInfo{});

//     detail::emplace_data(present_complete_semaphores, device);
//     detail::emplace_data(graphics_and_present_queue, present_complete_semaphores);

//     frames.emplace_back(command_buffer, in_flight_fence, present_complete_semaphores);
//   }
// }