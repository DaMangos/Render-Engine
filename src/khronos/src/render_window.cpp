#include "detail/make_shared_with_data.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

#include <glfw/window.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>

#include <algorithm>
#include <array>
#include <cstring>
#include <memory>
#include <ranges>
#include <stdexcept>

extern unsigned int  triangle_spv_len;
extern unsigned char triangle_spv[];

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

  auto const framebuffer = glfw::window::get_framebuffer_size();

  auto const surface_capabilities = physical_device->getSurfaceCapabilitiesKHR(*surface);

  auto const image_width = std::clamp<std::uint32_t>(static_cast<std::uint32_t>(framebuffer.width),
                                                     surface_capabilities.minImageExtent.width,
                                                     surface_capabilities.maxImageExtent.width);

  auto const image_height = std::clamp<std::uint32_t>(static_cast<std::uint32_t>(framebuffer.height),
                                                      surface_capabilities.minImageExtent.height,
                                                      surface_capabilities.maxImageExtent.height);

  logging::verbose() << "framebuffer width = " << framebuffer.width
                     << " and framebuffer height = " << framebuffer.height;

  auto const image_extent = vk::Extent2D{}.setWidth(image_width).setHeight(image_height);

  logging::verbose() << "image width = " << image_width << " and image height = " << image_height;

  if(default_swapchain_create_info->surface != **surface)
    throw std::runtime_error("render window "s + get_title()
                             + "'s surface differs from the surface used to find the physical device");

  swapchain_create_info
    = detail::make_shared_with_data<vk::SwapchainCreateInfoKHR>(*default_swapchain_create_info);

  detail::emplace_data(swapchain_create_info, default_swapchain_create_info);

  swapchain_create_info->setImageExtent(image_extent);

  swapchain = detail::make_shared_with_data<vk::raii::SwapchainKHR const>(*device, *swapchain_create_info);

  detail::emplace_data(swapchain, device);
  detail::emplace_data(swapchain, surface);

  auto const wait_for_queue = [=](vk::SwapchainKHR const &)
  {
    graphics_and_present_queue->waitIdle();
  };

  detail::emplace_function(swapchain, wait_for_queue);

  for(auto const & image : swapchain->getImages())
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

    auto const image_view
      = detail::make_shared_with_data<vk::raii::ImageView const>(*device, image_view_create_info);

    detail::emplace_data(image_view, swapchain);

    auto const render_complete_semaphores
      = detail::make_shared_with_data<vk::raii::Semaphore const>(*device, vk::SemaphoreCreateInfo{});

    detail::emplace_data(render_complete_semaphores, device);
    detail::emplace_data(graphics_and_present_queue, render_complete_semaphores);

    images.emplace_back(image, image_view, render_complete_semaphores);
  }

  assert(triangle_spv_len % sizeof(std::uint32_t) == 0);

  auto const code = std::make_unique_for_overwrite<std::uint32_t[]>(triangle_spv_len);

  std::memcpy(code.get(), triangle_spv, triangle_spv_len);

  auto const shader_module_create_info
    = vk::ShaderModuleCreateInfo{}.setCodeSize(triangle_spv_len).setPCode(code.get());

  shader_module = detail::make_shared_with_data<vk::raii::ShaderModule const>(
    device->createShaderModule(shader_module_create_info));

  detail::emplace_data(shader_module, device);

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

  auto const pipeline_dynamic_state_create_info
    = vk::PipelineDynamicStateCreateInfo{}.setDynamicStates(dynamic_states);

  auto const pipeline_vertex_input_state_create_info = vk::PipelineVertexInputStateCreateInfo{};

  auto const pipeline_input_assembly_state_create_info = vk::PipelineInputAssemblyStateCreateInfo{}
                                                           .setTopology(vk::PrimitiveTopology::eTriangleList)
                                                           .setPrimitiveRestartEnable(vk::False);

  auto const viewport = vk::Viewport{}
                          .setX(0.0f)
                          .setY(0.0f)
                          .setWidth(static_cast<float>(swapchain_create_info->imageExtent.width))
                          .setHeight(static_cast<float>(swapchain_create_info->imageExtent.height))
                          .setMinDepth(0.0f)
                          .setMaxDepth(1.0f);

  auto const scissor
    = vk::Rect2D{}.setOffset(vk::Offset2D{}.setX(0).setY(0)).setExtent(swapchain_create_info->imageExtent);

  auto const pipeline_viewport_state_create_info
    = vk::PipelineViewportStateCreateInfo{}.setViewports(viewport).setScissors(scissor);

  constexpr auto pipeline_rasterization_state_create_info = vk::PipelineRasterizationStateCreateInfo{}
                                                              .setDepthClampEnable(vk::False)
                                                              .setRasterizerDiscardEnable(vk::False)
                                                              .setPolygonMode(vk::PolygonMode::eFill)
                                                              .setCullMode(vk::CullModeFlagBits::eBack)
                                                              .setFrontFace(vk::FrontFace::eClockwise)
                                                              .setDepthBiasEnable(vk::False)
                                                              .setLineWidth(1.0f);

  constexpr auto pipeline_multisample_state_create_info
    = vk::PipelineMultisampleStateCreateInfo{}
        .setRasterizationSamples(vk::SampleCountFlagBits::e1)
        .setSampleShadingEnable(vk::False);

  constexpr auto pipeline_color_blend_attachment_states = std::array{
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

  auto const pipeline_color_blend_state_create_info
    = vk::PipelineColorBlendStateCreateInfo{}
        .setLogicOpEnable(vk::False)
        .setLogicOp(vk::LogicOp::eCopy)
        .setAttachments(pipeline_color_blend_attachment_states);

  auto const pipeline_layout_info = vk::PipelineLayoutCreateInfo{};

  pipeline_layout
    = detail::make_shared_with_data<vk::raii::PipelineLayout const>(*device, pipeline_layout_info);

  detail::emplace_data(pipeline_layout, device);

  auto const color_attachment_formats = std::array{swapchain_create_info->imageFormat};

  auto const & [graphics_pipeline_create_info, _] = vk::StructureChain{
    vk::GraphicsPipelineCreateInfo{}
      .setStages(pipeline_shader_stage_create_info)
      .setPVertexInputState(&pipeline_vertex_input_state_create_info)
      .setPInputAssemblyState(&pipeline_input_assembly_state_create_info)
      .setPViewportState(&pipeline_viewport_state_create_info)
      .setPRasterizationState(&pipeline_rasterization_state_create_info)
      .setPMultisampleState(&pipeline_multisample_state_create_info)
      .setPColorBlendState(&pipeline_color_blend_state_create_info)
      .setPDynamicState(&pipeline_dynamic_state_create_info)
      .setLayout(*pipeline_layout),
    vk::PipelineRenderingCreateInfo{}.setColorAttachmentFormats(color_attachment_formats)};

  graphics_pipeline = detail::make_shared_with_data<vk::raii::Pipeline const>(*device,
                                                                              nullptr,
                                                                              graphics_pipeline_create_info);

  detail::emplace_data(graphics_pipeline, pipeline_layout);

  auto command_pool_create_info = vk::CommandPoolCreateInfo{}
                                    .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                                    .setQueueFamilyIndex(graphics_and_present_queue_family_index);

  graphics_and_present_command_pool
    = detail::make_shared_with_data<vk::raii::CommandPool const>(*device, command_pool_create_info);

  detail::emplace_data(graphics_and_present_command_pool, device);

  auto const graphics_and_present_command_buffer_allocate_info
    = vk::CommandBufferAllocateInfo{}
        .setCommandPool(*graphics_and_present_command_pool)
        .setLevel(vk::CommandBufferLevel::ePrimary)
        .setCommandBufferCount(2);

  auto const create_shared_command_buffer = [&](vk::raii::CommandBuffer & command_buffer)
  {
    auto const shared_command_buffer
      = detail::make_shared_with_data<vk::raii::CommandBuffer const>(std::move(command_buffer));

    detail::emplace_data(shared_command_buffer, graphics_and_present_command_pool);
    detail::emplace_data(shared_command_buffer, graphics_pipeline);

    return shared_command_buffer;
  };

  for(auto && graphics_and_present_command_buffer :
      vk::raii::CommandBuffers(*device, graphics_and_present_command_buffer_allocate_info)
        | std::views::transform(create_shared_command_buffer))
  {
    auto const in_flight_fence_create_info
      = vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled);

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

    detail::emplace_function(graphics_and_present_command_buffer, wait_for_in_flight_fence);

    auto const present_complete_semaphores
      = detail::make_shared_with_data<vk::raii::Semaphore const>(*device, vk::SemaphoreCreateInfo{});

    detail::emplace_data(present_complete_semaphores, device);
    detail::emplace_data(graphics_and_present_queue, present_complete_semaphores);

    frames.emplace_back(graphics_and_present_command_buffer, in_flight_fence, present_complete_semaphores);
  }
}
