#include "create_swapchain.hpp"

#include "../make_shared_with_deleter_data.hpp"
#include "vulkan/vulkan.hpp"

std::shared_ptr<vk::raii::SwapchainKHR const> graphics::detail::vulkan::create_swapchain(
  std::shared_ptr<vk::raii::Device const> const &                                                   device,
  std::shared_ptr<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D> const> const & swapchain_data,
  int const                                                                                         framebuffer_width,
  int const                                                                                         framebuffer_height)
{
  auto const & [default_swapchain_create_info, min_image_extent, max_image_extent] = *swapchain_data;

  auto const image_width =
    std::clamp<uint32_t>(static_cast<uint32_t>(framebuffer_width), min_image_extent.width, max_image_extent.width);

  auto const image_height =
    std::clamp<uint32_t>(static_cast<uint32_t>(framebuffer_height), min_image_extent.height, max_image_extent.height);

  auto const image_extent = vk::Extent2D{}            //
                              .setWidth(image_width)  //
                              .setHeight(image_height);

  auto const swapchain_create_info = vk::SwapchainCreateInfoKHR{default_swapchain_create_info}.setImageExtent(image_extent);

  return make_shared_with_deleter_data<vk::raii::SwapchainKHR const>(std::make_tuple(device, swapchain_data),
                                                                     *device,
                                                                     swapchain_create_info);
}