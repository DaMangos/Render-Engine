#include "detail/make_shared_with_data.hpp"

#include <glfw/fwd.hpp>
#include <glfw/library.hpp>
#include <glfw/window.hpp>
#include <khronos/fwd.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <memory>
#include <ranges>
#include <stdexcept>

khronos::render_window::render_window(
  present_window &&                                         window,
  std::shared_ptr<vk::raii::PhysicalDevice const> const &   physical_device,
  std::shared_ptr<vk::raii::Device const> const &           device,
  std::shared_ptr<vk::SwapchainCreateInfoKHR const> const & default_swapchain_create_info,
  std::shared_ptr<vk::raii::Queue const> const &            graphics_and_present_queue,
  std::uint32_t const                                       graphics_and_present_queue_family_index)
: present_window(std::move(window))
{
  using namespace std::literals;

  if(default_swapchain_create_info->surface != **surface)
    throw std::runtime_error("render window "s + glfw::window::get_title()
                             + "'s surface differs from the surface used to find the physical device");

  swapchain_create_info = detail::make_shared_with_data<vk::SwapchainCreateInfoKHR>(*default_swapchain_create_info);

  detail::emplace_data(swapchain_create_info, default_swapchain_create_info);

  glfw::window::when_framebuffer_resized = [=](glfw::window & self, glfw::dimensions<int, 2> const & size)
  {
    auto & render_window_self = dynamic_cast<render_window &>(self);

    auto const surface_capabilities = physical_device->getSurfaceCapabilitiesKHR(*render_window_self.surface);

    auto const image_width = std::clamp<std::uint32_t>(static_cast<std::uint32_t>(size.width),
                                                       surface_capabilities.minImageExtent.width,
                                                       surface_capabilities.maxImageExtent.width);

    auto const image_height = std::clamp<std::uint32_t>(static_cast<std::uint32_t>(size.height),
                                                        surface_capabilities.minImageExtent.height,
                                                        surface_capabilities.maxImageExtent.height);

    auto const image_extent = vk::Extent2D{}.setWidth(image_width).setHeight(image_height);

    logging::verbose() << "image width = " << image_width << " and image height = " << image_height;

    render_window_self.swapchain_create_info
      = detail::make_shared_with_data<vk::SwapchainCreateInfoKHR>(*render_window_self.swapchain_create_info);

    detail::emplace_data(render_window_self.swapchain_create_info, render_window_self.surface);

    render_window_self.swapchain_create_info->setImageExtent(image_extent);

    if(render_window_self.swapchain)
    {
      detail::emplace_data(render_window_self.swapchain_create_info, render_window_self.swapchain);

      render_window_self.swapchain_create_info->setOldSwapchain(*render_window_self.swapchain);
    }

    render_window_self.swapchain
      = detail::make_shared_with_data<vk::raii::SwapchainKHR const>(*device, *render_window_self.swapchain_create_info);

    detail::emplace_data(render_window_self.swapchain, device);
    detail::emplace_data(render_window_self.swapchain, render_window_self.surface);
    detail::emplace_function(render_window_self.swapchain,
                             [=](auto const &) { graphics_and_present_queue->waitIdle(); });

    render_window_self.images.clear();

    for(auto const & image : render_window_self.swapchain->getImages())
    {
      constexpr auto image_subresource_range = vk::ImageSubresourceRange{}
                                                 .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                                 .setBaseMipLevel(0)
                                                 .setLevelCount(1)
                                                 .setBaseArrayLayer(0)
                                                 .setLayerCount(1);

      auto const image_view_create_info = vk::ImageViewCreateInfo{}
                                            .setViewType(vk::ImageViewType::e2D)
                                            .setFormat(render_window_self.swapchain_create_info->imageFormat)
                                            .setSubresourceRange(image_subresource_range)
                                            .setImage(image);

      auto const image_view = detail::make_shared_with_data<vk::raii::ImageView const>(*device, image_view_create_info);

      detail::emplace_data(image_view, render_window_self.swapchain);

      auto const render_complete_semaphores
        = detail::make_shared_with_data<vk::raii::Semaphore const>(*device, vk::SemaphoreCreateInfo{});

      detail::emplace_data(render_complete_semaphores, device);
      detail::emplace_data(graphics_and_present_queue, render_complete_semaphores);

      render_window_self.images.emplace_back(image, image_view, render_complete_semaphores);
    }
  };

  glfw::window::when_framebuffer_resized(*this, glfw::window::get_framebuffer_size());

  auto const command_pool_create_info = vk::CommandPoolCreateInfo{}
                                          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                                          .setQueueFamilyIndex(graphics_and_present_queue_family_index);

  command_pool = detail::make_shared_with_data<vk::raii::CommandPool const>(*device, command_pool_create_info);

  detail::emplace_data(command_pool, device);

  auto const command_buffer_allocate_info = vk::CommandBufferAllocateInfo{}
                                              .setCommandPool(*command_pool)
                                              .setLevel(vk::CommandBufferLevel::ePrimary)
                                              .setCommandBufferCount(2);

  auto const create_shared_command_buffer = [&](vk::raii::CommandBuffer & command_buffer)
  {
    auto const shared_command_buffer
      = detail::make_shared_with_data<vk::raii::CommandBuffer const>(std::move(command_buffer));

    detail::emplace_data(shared_command_buffer, command_pool);

    return shared_command_buffer;
  };

  for(auto && command_buffer : vk::raii::CommandBuffers(*device, command_buffer_allocate_info)
                                 | std::views::transform(create_shared_command_buffer))
  {
    auto const in_flight_fence_create_info = vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled);

    auto const in_flight_fence
      = detail::make_shared_with_data<vk::raii::Fence const>(*device, in_flight_fence_create_info);

    detail::emplace_data(in_flight_fence, device);
    detail::emplace_data(graphics_and_present_queue, in_flight_fence);

    auto const wait_for_in_flight_fence = [=](vk::raii::CommandBuffer const &)
    {
      auto const wait_for_fences_result
        = device->waitForFences(**in_flight_fence, vk::True, std::numeric_limits<std::uint64_t>::max());

      if(wait_for_fences_result < vk::Result::eSuccess)
        logging::error() << "wait for fences returned a error: " << vk::to_string(wait_for_fences_result);

      if(wait_for_fences_result > vk::Result::eSuccess)
        logging::warning() << "wait for fences returned a warring: " << vk::to_string(wait_for_fences_result);
    };

    detail::emplace_function(command_buffer, wait_for_in_flight_fence);

    auto const present_complete_semaphores
      = detail::make_shared_with_data<vk::raii::Semaphore const>(*device, vk::SemaphoreCreateInfo{});

    detail::emplace_data(present_complete_semaphores, device);
    detail::emplace_data(graphics_and_present_queue, present_complete_semaphores);

    frames.emplace_back(command_buffer, in_flight_fence, present_complete_semaphores);
  }
}
