#include "detail/create_swapchain_create_info.hpp"
#include "detail/make_shared_with_data.hpp"

#include <glfw/window.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>

#include <iomanip>
#include <memory>
#include <stdexcept>

khronos::render_window::render_window(
  present_window &&                                         window,
  std::shared_ptr<vk::raii::PhysicalDevice const> const &   physical_device,
  std::shared_ptr<vk::raii::Device const> const &           device,
  std::shared_ptr<vk::SwapchainCreateInfoKHR const> const & default_swapchain_create_info,
  std::shared_ptr<vk::Extent2D const> const &               min_image_extent,
  std::shared_ptr<vk::Extent2D const> const &               max_image_extent)
: present_window(std::move(window))
{
  auto const framebuffer = get_framebuffer_size();

  auto const image_width = std::clamp<std::uint32_t>(static_cast<std::uint32_t>(framebuffer.width),
                                                     min_image_extent->width,
                                                     max_image_extent->width);

  auto const image_height = std::clamp<std::uint32_t>(static_cast<std::uint32_t>(framebuffer.height),
                                                      min_image_extent->height,
                                                      max_image_extent->height);

  auto const image_extent = vk::Extent2D{}.setWidth(image_width).setHeight(image_height);

  if(default_swapchain_create_info->surface != **surface)
  {
    logging::verbose() << "render window " << std::quoted(get_title())
                       << "'s surface differs from the surface used to find the physical device";

    auto const new_swapchain_create_info = detail::create_swapchain_create_info(physical_device, surface);

    if(not new_swapchain_create_info)
      throw std::runtime_error(
        "cannot create render window as there are no suitable physical devices for specified surface");

    swapchain_create_info = vk::SwapchainCreateInfoKHR{*new_swapchain_create_info}.setImageExtent(image_extent);
  }
  else
    swapchain_create_info = vk::SwapchainCreateInfoKHR{*default_swapchain_create_info}.setImageExtent(image_extent);

  swapchain = detail::make_shared_with_data<vk::raii::SwapchainKHR const>(std::make_tuple(device, surface),
                                                                          *device,
                                                                          *swapchain_create_info);

  auto const images = swapchain->getImages();

  image_views.reserve(images.size());

  for(auto const & image : images)
  {
    constexpr auto image_subresource_range = vk::ImageSubresourceRange{}
                                               .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                               .setBaseMipLevel(0)
                                               .setLevelCount(1)
                                               .setBaseArrayLayer(0)
                                               .setLayerCount(1);

    auto const image_view_create_info = vk::ImageViewCreateInfo{}
                                          .setViewType(vk::ImageViewType::e2D)
                                          .setFormat(swapchain_create_info->imageFormat)
                                          .setSubresourceRange(image_subresource_range)
                                          .setImage(image);

    image_views.emplace_back(
      detail::make_shared_with_data<vk::raii::ImageView const>(std::make_tuple(device, swapchain),
                                                               *device,
                                                               image_view_create_info));
  }
}