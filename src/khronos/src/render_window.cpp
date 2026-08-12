#include "detail/create_swapchain_create_info.hpp"
#include "detail/make_shared_with_data.hpp"
#include "vulkan/vulkan_raii.hpp"

#include <glfw/window.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>

#include <array>
#include <cstring>
#include <iomanip>
#include <memory>
#include <stdexcept>

extern unsigned int  triangle_spv_len;
extern unsigned char triangle_spv[];

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

    swapchain_create_info = detail::create_swapchain_create_info(physical_device, surface);

    if(not swapchain_create_info)
      throw std::runtime_error(
        "cannot create render window as there are no suitable physical devices for specified surface");
  }
  else
    swapchain_create_info = detail::make_shared_with_data<vk::SwapchainCreateInfoKHR>(surface,
                                                                                      *default_swapchain_create_info);

  swapchain_create_info->setImageExtent(image_extent);

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

    image_views
      .emplace_back(detail::make_shared_with_data<vk::raii::ImageView const>(std::make_tuple(device, swapchain),
                                                                             *device,
                                                                             image_view_create_info));
  }

  if(triangle_spv_len % sizeof(std::uint32_t))
    throw;

  auto const code = std::make_unique_for_overwrite<std::uint32_t[]>(triangle_spv_len);

  std::memcpy(code.get(), triangle_spv, triangle_spv_len);

  auto const shader_module_create_info = vk::ShaderModuleCreateInfo{}
                                           .setCodeSize(triangle_spv_len)
                                           .setPCode(code.get());

  shader_module = detail::make_shared_with_data<vk::raii::ShaderModule const>(device,
                                                                              device->createShaderModule(
                                                                                shader_module_create_info));

  auto const pipeline_shader_stage_create_info = std::array{
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eVertex)
      .setModule(*shader_module)
      .setPName("vert_main"),
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eFragment)
      .setModule(*shader_module)
      .setPName("frag_main"),
  };

  auto const dynamic_states = std::array{vk::DynamicState::eViewport, vk::DynamicState::eScissor};

  auto const pipeline_dynamic_state_create_info = vk::PipelineDynamicStateCreateInfo{}.setDynamicStates(dynamic_states);

  auto const pipeline_vertex_input_state_create_info = vk::PipelineVertexInputStateCreateInfo{};

  auto const pipeline_input_assembly_state_create_info = vk::PipelineInputAssemblyStateCreateInfo{}
                                                           .setTopology(vk::PrimitiveTopology::eTriangleList)
                                                           .setPrimitiveRestartEnable(vk::False);

  auto const viewports = std::array{
    vk::Viewport{}
      .setX(0.0f)
      .setY(0.0f)
      .setWidth(static_cast<float>(image_extent.width))
      .setHeight(static_cast<float>(image_extent.width))
      .setMinDepth(0.0f)
      .setMaxDepth(1.0f),
  };

  auto const scissors = std::array{
    vk::Rect2D{}.setOffset(vk::Offset2D{}.setX(0).setY(0)).setExtent(image_extent),
  };

  auto const pipeline_viewport_state_create_info = vk::PipelineViewportStateCreateInfo{}
                                                     .setViewports(viewports)
                                                     .setScissors(scissors);

  auto const pipeline_rasterization_state_create_info = vk::PipelineRasterizationStateCreateInfo{}
                                                          .setDepthClampEnable(vk::False)
                                                          .setRasterizerDiscardEnable(vk::False)
                                                          .setPolygonMode(vk::PolygonMode::eFill)
                                                          .setCullMode(vk::CullModeFlagBits::eBack)
                                                          .setFrontFace(vk::FrontFace::eClockwise)
                                                          .setDepthBiasEnable(vk::False)
                                                          .setLineWidth(1.0f);

  auto const pipeline_multisample_state_create_info = vk::PipelineMultisampleStateCreateInfo{}
                                                        .setRasterizationSamples(vk::SampleCountFlagBits::e1)
                                                        .setSampleShadingEnable(vk::False);

  auto const pipeline_color_blend_attachment_states = std::array{
    vk::PipelineColorBlendAttachmentState{}
      .setBlendEnable(vk::True)
      .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
      .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
      .setColorBlendOp(vk::BlendOp::eAdd)
      .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
      .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
      .setAlphaBlendOp(vk::BlendOp::eAdd)
      .setColorWriteMask(vk::ColorComponentFlagBits::eR    //
                         | vk::ColorComponentFlagBits::eG  //
                         | vk::ColorComponentFlagBits::eB  //
                         | vk::ColorComponentFlagBits::eA),
  };

  auto const pipeline_color_blend_state_create_info = vk::PipelineColorBlendStateCreateInfo{}
                                                        .setLogicOpEnable(vk::False)
                                                        .setLogicOp(vk::LogicOp::eCopy)
                                                        .setAttachments(pipeline_color_blend_attachment_states);

  auto const pipeline_layout_info = vk::PipelineLayoutCreateInfo{};

  pipeline_layout = detail::make_shared_with_data<vk::raii::PipelineLayout>(device, *device, pipeline_layout_info);

  auto const color_attachment_formats = std::array{swapchain_create_info->imageFormat};

  auto const & [graphics_pipeline_create_info,
                _] = vk::StructureChain{vk::GraphicsPipelineCreateInfo{}
                                          .setStages(pipeline_shader_stage_create_info)
                                          .setPVertexInputState(&pipeline_vertex_input_state_create_info)
                                          .setPInputAssemblyState(&pipeline_input_assembly_state_create_info)
                                          .setPViewportState(&pipeline_viewport_state_create_info)
                                          .setPRasterizationState(&pipeline_rasterization_state_create_info)
                                          .setPMultisampleState(&pipeline_multisample_state_create_info)
                                          .setPColorBlendState(&pipeline_color_blend_state_create_info)
                                          .setPDynamicState(&pipeline_dynamic_state_create_info)
                                          .setLayout(*pipeline_layout),
                                        vk::PipelineRenderingCreateInfo{}
                                          .setColorAttachmentFormats(color_attachment_formats)};

  graphics_pipeline = detail::make_shared_with_data<vk::raii::Pipeline const>(device,
                                                                              *device,
                                                                              nullptr,
                                                                              graphics_pipeline_create_info);
}