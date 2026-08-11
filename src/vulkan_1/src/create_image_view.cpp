// #include "create_image_view.hpp"

// #include "../make_shared_with_data.hpp"
// #include "vulkan_1/vulkan_raii.hpp"

// std::vector<std::shared_ptr<vk::raii::ImageView const>> vulkan_1::create_image_view(
//   std::shared_ptr<vk::raii::Device const> const & device, std::shared_ptr<vk::raii::SwapchainKHR
//   const> const &                                             swapchain,
//   std::shared_ptr<std::tuple<vk::SwapchainCreateInfoKHR, vk::Extent2D, vk::Extent2D> const> const
//   & swapchain_data)
// {
//   std::vector<std::shared_ptr<vk::raii::ImageView const>> image_view;

//   auto const images = swapchain->getImages();

//   image_view.reserve(images.size());

//   for(auto const & image : images)
//   {
//     constexpr auto image_subresource_range = vk::ImageSubresourceRange{} //
//                                                .setAspectMask(vk::ImageAspectFlagBits::eColor) //
//                                                .setBaseMipLevel(0) // .setLevelCount(1) //
//                                                .setBaseArrayLayer(0) // .setLayerCount(1);

//     auto const image_view_create_info = vk::ImageViewCreateInfo{} //
//                                           .setViewType(vk::ImageViewType::e2D) //
//                                           .setFormat(std::get<0>(*swapchain_data).imageFormat) //
//                                           .setSubresourceRange(image_subresource_range) //
//                                           .setImage(image);

//     image_view.emplace_back(
//       make_shared_with_data<vk::raii::ImageView const>(std::make_tuple(device,
//       swapchain), *device, image_view_create_info));
//   }

//   return image_view;
// };
