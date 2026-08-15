#include "detail/make_shared_with_data.hpp"

#include <glfw/def.hpp>
#include <glfw/library.hpp>
#include <glfw/window.hpp>
#include <khronos/def.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <memory>
#include <ranges>
#include <stdexcept>

extern unsigned int  shader_spv_len;
extern unsigned char shader_spv[];

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

  swapchain_create_info
    = detail::make_shared_with_data<vk::SwapchainCreateInfoKHR>(*default_swapchain_create_info);

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

    render_window_self.swapchain = detail::make_shared_with_data<vk::raii::SwapchainKHR const>(
      *device,
      *render_window_self.swapchain_create_info);

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

      auto const image_view
        = detail::make_shared_with_data<vk::raii::ImageView const>(*device, image_view_create_info);

      detail::emplace_data(image_view, render_window_self.swapchain);

      auto const render_complete_semaphores
        = detail::make_shared_with_data<vk::raii::Semaphore const>(*device, vk::SemaphoreCreateInfo{});

      detail::emplace_data(render_complete_semaphores, device);
      detail::emplace_data(graphics_and_present_queue, render_complete_semaphores);

      render_window_self.images.emplace_back(image, image_view, render_complete_semaphores);
    }
  };

  glfw::window::when_framebuffer_resized(*this, glfw::window::get_framebuffer_size());

  assert(shader_spv_len % sizeof(std::uint32_t) == 0);

  auto const code = std::make_unique_for_overwrite<std::uint32_t[]>(shader_spv_len);

  std::memcpy(code.get(), shader_spv, shader_spv_len);

  auto const shader_module_create_info
    = vk::ShaderModuleCreateInfo{}.setCodeSize(shader_spv_len).setPCode(code.get());

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

  auto const vertex_input_attribute_description = std::array{
    vk::VertexInputAttributeDescription{}
      .setLocation(0)
      .setBinding(0)
      .setFormat(vk::Format::eR32G32Sfloat)
      .setOffset(offsetof(vertex, pos)),
    vk::VertexInputAttributeDescription{}
      .setLocation(1)
      .setBinding(0)
      .setFormat(vk::Format::eR32G32B32Sfloat)
      .setOffset(offsetof(vertex, colour)),
  };

  auto const vertex_input_binding_description = vk::VertexInputBindingDescription{}
                                                  .setBinding(0)
                                                  .setInputRate(vk::VertexInputRate::eVertex)
                                                  .setStride(sizeof(vertex));

  auto const pipeline_vertex_input_state_create_info
    = vk::PipelineVertexInputStateCreateInfo{}
        .setVertexAttributeDescriptions(vertex_input_attribute_description)
        .setVertexBindingDescriptions(vertex_input_binding_description);

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

  graphics_pipeline_layout
    = detail::make_shared_with_data<vk::raii::PipelineLayout const>(*device, pipeline_layout_info);

  detail::emplace_data(graphics_pipeline_layout, device);

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
      .setLayout(*graphics_pipeline_layout),
    vk::PipelineRenderingCreateInfo{}.setColorAttachmentFormats(color_attachment_formats)};

  graphics_pipeline = detail::make_shared_with_data<vk::raii::Pipeline const>(*device,
                                                                              nullptr,
                                                                              graphics_pipeline_create_info);

  detail::emplace_data(graphics_pipeline, graphics_pipeline_layout);

  auto const vertex_buffer_create_info = vk::BufferCreateInfo{}
                                           .setSize(sizeof(vertices.front()) * vertices.size())
                                           .setSharingMode(vk::SharingMode::eExclusive)
                                           .setUsage(vk::BufferUsageFlagBits::eVertexBuffer);

  vertex_buffer = detail::make_shared_with_data<vk::raii::Buffer const>(*device, vertex_buffer_create_info);

  detail::emplace_data(vertex_buffer, device);

  auto const memory_properties   = physical_device->getMemoryProperties();
  auto const memory_requirements = vertex_buffer->getMemoryRequirements();

  std::uint32_t memory_type_index = 0;
  for(; memory_type_index < memory_properties.memoryTypeCount; memory_type_index++)
  {
    auto const & memory_type = memory_properties.memoryTypes[memory_type_index];

    if(memory_requirements.memoryTypeBits & (1u << memory_type_index)
       and memory_type.propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible)
      break;
  }

  if(memory_type_index == memory_properties.memoryTypeCount)
    throw std::runtime_error("failed to find suitable device memory");

  logging::verbose() << "device memory type index " << memory_type_index;

  auto const memory_allocate_info = vk::MemoryAllocateInfo{}
                                      .setMemoryTypeIndex(memory_type_index)
                                      .setAllocationSize(memory_requirements.size);

  vertex_buffer_device_memory
    = detail::make_shared_with_data<vk::raii::DeviceMemory const>(*device, memory_allocate_info);

  detail::emplace_data(vertex_buffer_device_memory, vertex_buffer);

  vertex_buffer->bindMemory(**vertex_buffer_device_memory, 0);

  void * const data = vertex_buffer_device_memory->mapMemory(0, vertex_buffer_create_info.size);

  std::memcpy(data, vertices.data(), vertex_buffer_create_info.size);

  vertex_buffer_device_memory->unmapMemory();

  // auto const mapped_memory_range
  //   = vk::MappedMemoryRange{}.setMemory(*vertex_buffer_device_memory).setOffset(0).setSize(vk::WholeSize);

  // device->flushMappedMemoryRanges(mapped_memory_range);
  // device->invalidateMappedMemoryRanges(mapped_memory_range);

  auto const command_pool_create_info = vk::CommandPoolCreateInfo{}
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
