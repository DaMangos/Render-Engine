
#include "dependencies.hpp"
#include "impl.hpp"
#include "khronos/present_window.hpp"
#include "vulkan/vulkan_raii.hpp"

#include <glfw/window.hpp>
#include <khronos/graphical_device.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <memory>
#include <vector>

namespace
{
[[nodiscard]]
static constexpr std::uint32_t get_min_image_count(vk::SurfaceCapabilitiesKHR const & surface_capabilities) noexcept
{
  return surface_capabilities.maxImageCount == 0
         ? std::max(3u, surface_capabilities.minImageCount)
         : std::clamp(3u, surface_capabilities.minImageCount, surface_capabilities.maxImageCount);
}

[[nodiscard]]
static constexpr vk::Extent2D get_image_extent(glfw::dimensions<int, 2> const &   size,
                                               vk::SurfaceCapabilitiesKHR const & surface_capabilities) noexcept
{
  return vk::Extent2D{}
    .setWidth(std::clamp(static_cast<std::uint32_t>(size.width),
                         surface_capabilities.minImageExtent.width,
                         surface_capabilities.maxImageExtent.width))
    .setHeight(std::clamp(static_cast<std::uint32_t>(size.height),
                          surface_capabilities.minImageExtent.height,
                          surface_capabilities.maxImageExtent.height));
}

[[nodiscard]]
static constexpr std::uint32_t get_image_array_layers(vk::SurfaceCapabilitiesKHR const & surface_capabilities) noexcept
{
  return surface_capabilities.maxImageArrayLayers == 0 ? 1u : std::min(1u, surface_capabilities.maxImageArrayLayers);
}

[[nodiscard]]
static vk::SwapchainCreateInfoKHR get_swapchain_create_info(glfw::dimensions<int, 2> const &   size,
                                                            vk::SurfaceCapabilitiesKHR const & surface_capabilities,
                                                            vk::raii::SurfaceKHR const &       surface)
{
  return vk::SwapchainCreateInfoKHR{}
    .setSurface(*surface)
    .setMinImageCount(get_min_image_count(surface_capabilities))
    .setImageExtent(get_image_extent(size, surface_capabilities))
    .setImageArrayLayers(get_image_array_layers(surface_capabilities.maxImageArrayLayers))
    .setImageSharingMode(vk::SharingMode::eExclusive)
    .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
    .setPreTransform(surface_capabilities.currentTransform)
    .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
    .setPresentMode(vk::PresentModeKHR::eFifo)
    .setImageFormat(vk::Format::eB8G8R8A8Srgb)
    .setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear)
    .setClipped(vk::True);
}

[[nodiscard]]
static vk::SwapchainCreateInfoKHR get_swapchain_create_info(glfw::dimensions<int, 2> const &   size,
                                                            vk::SurfaceCapabilitiesKHR const & surface_capabilities,
                                                            vk::SwapchainCreateInfoKHR const & old_swapchain_create_info,
                                                            vk::raii::SwapchainKHR const &     old_swapchain)
{
  return vk::SwapchainCreateInfoKHR{old_swapchain_create_info}
    .setImageExtent(get_image_extent(size, surface_capabilities))
    .setOldSwapchain(*old_swapchain);
}

[[nodiscard]]
static std::vector<vk::raii::ImageView> create_image_views(vk::raii::Device const &           device,
                                                           std::vector<vk::Image> const &     images,
                                                           vk::SwapchainCreateInfoKHR const & swapchain_create_info)
{
  constexpr auto image_subresource_range = vk::ImageSubresourceRange{}
                                             .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                             .setBaseMipLevel(0)
                                             .setLevelCount(1)
                                             .setBaseArrayLayer(0)
                                             .setLayerCount(1);

  std::vector<vk::raii::ImageView> image_views;

  for(auto const & image : images)
  {
    auto const image_view_create_info = vk::ImageViewCreateInfo{}
                                          .setViewType(vk::ImageViewType::e2D)
                                          .setFormat(swapchain_create_info.imageFormat)
                                          .setSubresourceRange(image_subresource_range)
                                          .setImage(image);

    image_views.emplace_back(device, image_view_create_info);
  }

  return image_views;
}

static void wait_for_queue_no_throw(vk::raii::SwapchainKHR const &, vk::raii::Queue const & queue) noexcept
{
  try
  {
    queue.waitIdle();
  }
  catch(vk::Error const & error)
  {
    logging::error() << error.what();
  }
}

[[nodiscard]]
static khronos::render_window_impl create_render_window_impl(glfw::dimensions<int, 2> const & size,
                                                             khronos::surface const &         surface,
                                                             khronos::queue const &           queue)
{
  auto const physical_device_surface_info = vk::PhysicalDeviceSurfaceInfo2KHR{}.setSurface(surface.get());

  auto const surface_capabilities = queue.template get_dependency<vk::raii::PhysicalDevice>()
                                      .getSurfaceCapabilities2KHR(physical_device_surface_info)
                                      .surfaceCapabilities;

  auto swapchain_create_info = khronos::swapchain_create_info{surface.as_dependencies(),
                                                              get_swapchain_create_info(size, surface_capabilities, surface)};

  auto const swapchain_dependencies = khronos::dependency_union(swapchain_create_info.as_dependencies(),
                                                                queue.as_dependencies());

  auto swapchain = khronos::swapchain{wait_for_queue_no_throw,
                                      swapchain_dependencies,
                                      queue.template get_dependency<vk::raii::Device>(),
                                      swapchain_create_info.get()};

  auto images = swapchain.get().getImages();

  auto image_views = khronos::image_views{
    swapchain.as_dependencies(),
    create_image_views(queue.template get_dependency<vk::raii::Device>(), images, swapchain_create_info.get())};

  return {std::move(surface_capabilities),
          std::move(swapchain_create_info),
          std::move(swapchain),
          std::move(images),
          std::move(image_views)};
}
}

khronos::render_window::render_window(present_window && window, graphical_device const & graphical_device)
: present_window(std::move(window)),
  self(std::make_unique<render_window_impl>(create_render_window_impl(glfw::window::get_framebuffer_size(),
                                                                      present_window::self->surface,
                                                                      graphical_device.self->queue)))
{
}

khronos::render_window::~render_window() noexcept = default;

void khronos::render_window::when_framebuffer_resized(glfw::dimensions<int, 2> const & size)
{
  self->swapchain_create_info
    .get() = get_swapchain_create_info(size, self->surface_capabilities, self->swapchain_create_info.get(), self->swapchain);

  self->swapchain.template get_dependency<vk::raii::Queue>().waitIdle();

  self->swapchain.get() = {self->swapchain.template get_dependency<vk::raii::Device>(), self->swapchain_create_info.get()};

  self->images = self->swapchain.get().getImages();

  self->image_views.get() = create_image_views(self->image_views.template get_dependency<vk::raii::Device>(),
                                               self->images,
                                               self->swapchain_create_info.get());
}
