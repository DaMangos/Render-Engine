#include "dependencies.hpp"
#include "device_memory_resource.hpp"
#include "graphical_device_impl.hpp"
#include "graphics_pipeline_impl.hpp"
#include "mesh_buffer_impl.hpp"
#include "render_window_impl.hpp"
#include "vulakn_handles.hpp"

#include <graphics/graphical_device.hpp>
#include <graphics/graphics_pipeline.hpp>
#include <graphics/mesh_buffer.hpp>
#include <graphics/render_window.hpp>
#include <logging/logging.hpp>

#include <cstddef>
#include <exception>
#include <iterator>
#include <ranges>
#include <vector>

extern unsigned int  task_spv_len;
extern unsigned char task_spv[];

extern unsigned int  mesh_spv_len;
extern unsigned char mesh_spv[];

extern unsigned int  fragment_spv_len;
extern unsigned char fragment_spv[];

namespace
{
[[nodiscard]]
static graphics::vulkan::pipeline_layout create_pipeline_layout(
  graphics::vulkan::pipeline_layout::dependencies_type const & pipeline_layout_dependencies)
{
  return {pipeline_layout_dependencies, pipeline_layout_dependencies.get<vk::raii::Device>(), vk::PipelineLayoutCreateInfo{}};
}

[[nodiscard]]
static std::vector<std::uint32_t> create_shader_binary(unsigned int const spv_len, unsigned char const spv[])
{
  std::vector<std::uint32_t> shader_binary;

  shader_binary.resize(spv_len / sizeof(std::uint32_t));

  std::memcpy(shader_binary.data(), spv, spv_len);

  return shader_binary;
}

[[nodiscard]]
static graphics::vulkan::pipeline create_pipeline(graphics::vulkan::pipeline::dependencies_type const & pipeline_dependencies)
{
  auto const task_shader = create_shader_binary(task_spv_len, task_spv);

  auto const task_shader_module = vk::raii::ShaderModule{pipeline_dependencies.get<vk::raii::Device>(),
                                                         vk::ShaderModuleCreateInfo{}.setCode(task_shader)};

  auto const mesh_shader = create_shader_binary(mesh_spv_len, mesh_spv);

  auto const mesh_shader_module = vk::raii::ShaderModule{pipeline_dependencies.get<vk::raii::Device>(),
                                                         vk::ShaderModuleCreateInfo{}.setCode(mesh_spv_len)};

  auto const fragment_shader = create_shader_binary(fragment_spv_len, fragment_spv);

  auto const fragment_shader_module = vk::raii::ShaderModule{pipeline_dependencies.get<vk::raii::Device>(),
                                                             vk::ShaderModuleCreateInfo{}.setCode(fragment_shader)};

  auto const pipeline_shader_stage_create_info = std::array{
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eTaskEXT)
      .setModule(task_shader_module)
      .setPName("main"),
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eMeshEXT)
      .setModule(mesh_shader_module)
      .setPName("main"),
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eFragment)
      .setModule(fragment_shader_module)
      .setPName("main"),
  };

  constexpr auto pipeline_viewport_state_create_info = vk::PipelineViewportStateCreateInfo{}  //
                                                         .setViewportCount(1)                 //
                                                         .setScissorCount(1);

  constexpr auto pipeline_rasterization_state_create_info = vk::PipelineRasterizationStateCreateInfo{}
                                                              .setDepthClampEnable(vk::False)
                                                              .setRasterizerDiscardEnable(vk::False)
                                                              .setPolygonMode(vk::PolygonMode::eFill)
                                                              .setCullMode(vk::CullModeFlagBits::eBack)
                                                              .setFrontFace(vk::FrontFace::eClockwise)
                                                              .setDepthBiasEnable(vk::False)
                                                              .setLineWidth(1.0f);

  constexpr auto pipeline_multisample_state_create_info = vk::PipelineMultisampleStateCreateInfo{}
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

  auto const pipeline_color_blend_state_create_info = vk::PipelineColorBlendStateCreateInfo{}
                                                        .setLogicOpEnable(vk::False)
                                                        .setLogicOp(vk::LogicOp::eCopy)
                                                        .setAttachments(pipeline_color_blend_attachment_states);

  constexpr auto dynamic_states = std::array{vk::DynamicState::eViewport, vk::DynamicState::eScissor};

  auto const pipeline_dynamic_state_create_info = vk::PipelineDynamicStateCreateInfo{}.setDynamicStates(dynamic_states);

  constexpr auto format = vk::Format::eB8G8R8A8Srgb;

  auto const graphics_pipeline_create_info = vk::StructureChain{
    vk::GraphicsPipelineCreateInfo{}
      .setStages(pipeline_shader_stage_create_info)
      .setPViewportState(&pipeline_viewport_state_create_info)
      .setPRasterizationState(&pipeline_rasterization_state_create_info)
      .setPMultisampleState(&pipeline_multisample_state_create_info)
      .setPColorBlendState(&pipeline_color_blend_state_create_info)
      .setPDynamicState(&pipeline_dynamic_state_create_info)
      .setLayout(pipeline_dependencies.get<vk::raii::PipelineLayout>()),
    vk::PipelineRenderingCreateInfo{}  //
      .setColorAttachmentFormats(format)};

  return {
    pipeline_dependencies,
    pipeline_dependencies.get<vk::raii::Device>(),
    nullptr,
    graphics_pipeline_create_info.get(),
  };
}

[[nodiscard]]
static graphics::vulkan::command_pool create_command_pool(
  graphics::vulkan::command_pool::dependencies_type const & command_pool_dependencies,
  std::uint32_t const                                       queue_family_index)
{
  auto const command_pool_create_info = vk::CommandPoolCreateInfo{}
                                          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                                          .setQueueFamilyIndex(queue_family_index);

  return {command_pool_dependencies, command_pool_dependencies.get<vk::raii::Device>(), command_pool_create_info};
}

[[nodiscard]]
static graphics::vulkan::semaphores create_semaphores(
  graphics::vulkan::semaphores::dependencies_type const & semaphores_dependencies,
  std::size_t const                                       count)
{
  auto semaphores = graphics::vulkan::semaphores{semaphores_dependencies};

  for(std::size_t i = 0; i < count; i++)
    semaphores.get().emplace_back(semaphores_dependencies.get<vk::raii::Device>(), vk::SemaphoreCreateInfo{});

  return semaphores;
}

[[nodiscard]]
static graphics::vulkan::fences create_fences(graphics::vulkan::fences::dependencies_type const & fences_dependencies,
                                              std::size_t const                                   count)
{
  auto fences = graphics::vulkan::fences{fences_dependencies};

  constexpr auto fence_create_info = vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled);

  for(std::size_t i = 0; i < count; i++)
    fences.get().emplace_back(fences_dependencies.get<vk::raii::Device>(), fence_create_info);

  return fences;
}

void wait_for_fences_no_throw(vk::raii::Device const & device, std::list<vk::raii::Fence> const & fences) noexcept
{
  if(fences.empty())
    return;

  try
  {
    try
    {
      auto const flattened_fences = fences                                                            //
                                  | std::views::transform([](auto const & fence) { return *fence; })  //
                                  | std::ranges::to<std::vector>();

      auto const result = device.waitForFences(flattened_fences, vk::True, std::numeric_limits<std::uint64_t>::max());

      if(result > vk::Result::eSuccess)
        logging::warning() << "wait for fences returned a warning: " << vk::to_string(result);

      device.resetFences(flattened_fences);
    }
    catch(std::exception const & error)
    {
      logging::error() << error.what();
    }
  }
  catch(...)
  {
  }
}

template <class... Fences>
void wait_for_fences(vk::raii::Device const & device, vk::raii::Fence const & fence, Fences const &... fences)
{
  auto const flattened_fences = std::array{*fence, *fences...};

  auto const result = device.waitForFences(flattened_fences, vk::True, std::numeric_limits<std::uint64_t>::max());

  if(result > vk::Result::eSuccess)
    logging::warning() << "wait for fences returned a warning: " << vk::to_string(result);

  device.resetFences(flattened_fences);
}

[[nodiscard]]
static graphics::vulkan::command_buffers allocate_command_buffers(
  graphics::vulkan::command_buffers::dependencies_type const & command_buffers_dependencies,
  std::uint32_t const                                          count)
{
  auto const command_buffer_allocate_info = vk::CommandBufferAllocateInfo{}
                                              .setCommandPool(command_buffers_dependencies.get<vk::raii::CommandPool>())
                                              .setLevel(vk::CommandBufferLevel::ePrimary)
                                              .setCommandBufferCount(count);

  auto command_buffers = vk::raii::CommandBuffers{command_buffers_dependencies.get<vk::raii::Device>(),
                                                  command_buffer_allocate_info};

  return {
    wait_for_fences_no_throw,
    command_buffers_dependencies,
    std::make_move_iterator(command_buffers.begin()),
    std::make_move_iterator(command_buffers.end()),
  };
}

[[nodiscard]]
static graphics::graphics_pipeline_impl create_graphics_pipeline_impl(graphics::vulkan::queue const & queue,
                                                                      std::uint32_t const             queue_family_index)
{
  auto pipeline_layout = create_pipeline_layout(queue.get_dependencies());

  auto pipeline = create_pipeline(pipeline_layout.as_dependencies());

  auto command_pool = create_command_pool(queue.as_dependencies(), queue_family_index);

  constexpr auto transfer_cycles = 2uz;

  auto transfer_complete_fences = create_fences(queue.as_dependencies(), transfer_cycles);

  auto const transfer_command_buffers_dependencies = graphics::dependency_union(command_pool.as_dependencies(),
                                                                                transfer_complete_fences.as_dependencies());

  auto transfer_command_buffers = allocate_command_buffers(transfer_command_buffers_dependencies, transfer_cycles);

  constexpr auto frames_in_flight = 2uz;

  auto render_complete_semaphores = create_semaphores(queue.as_dependencies(), frames_in_flight);

  auto present_complete_semaphores = create_semaphores(queue.as_dependencies(), frames_in_flight);

  auto draw_complete_fences = create_fences(queue.as_dependencies(), frames_in_flight);

  auto const draw_command_buffers_dependencies = graphics::dependency_union(command_pool.as_dependencies(),
                                                                            draw_complete_fences.as_dependencies());

  auto draw_command_buffers = allocate_command_buffers(draw_command_buffers_dependencies, frames_in_flight);

  return {
    std::move(pipeline_layout),
    std::move(pipeline),
    std::move(command_pool),
    {},
    std::move(transfer_complete_fences),
    std::move(transfer_command_buffers),
    std::move(render_complete_semaphores),
    std::move(present_complete_semaphores),
    std::move(draw_complete_fences),
    std::move(draw_command_buffers),
  };
}
}

graphics::graphics_pipeline::graphics_pipeline(graphical_device const & graphical_device)
: self(std::make_unique<graphics_pipeline_impl>(
    create_graphics_pipeline_impl(graphical_device.self->queue, graphical_device.self->queue_family_index)))
{
}

graphics::graphics_pipeline::~graphics_pipeline() noexcept = default;

void graphics::graphics_pipeline::attach(mesh_buffer const & mesh_buffer)
{
  self->transfer_complete_fences.get().splice(self->transfer_complete_fences.get().end(),
                                              self->transfer_complete_fences.get(),
                                              self->transfer_complete_fences.get().begin());

  self->transfer_command_buffers.get().splice(self->transfer_command_buffers.get().end(),
                                              self->transfer_command_buffers.get(),
                                              self->transfer_command_buffers.get().begin());

  wait_for_fences(self->transfer_complete_fences.get_dependency<vk::raii::Device>(),
                  self->transfer_complete_fences.get().front());

  auto const staging_buffers = std::array{
    mesh_buffer.self->device_memory_resource->find_host_visible_buffer(mesh_buffer.self->vertex_buffer.data()),
    mesh_buffer.self->device_memory_resource->find_host_visible_buffer(mesh_buffer.self->index_buffer.data()),
    mesh_buffer.self->device_memory_resource->find_host_visible_buffer(mesh_buffer.self->texture_buffer.data()),
  };

  auto const allocation_sizes = std::array{
    vk::DeviceSize{staging_buffers[0].get_mapped_memory().size()},
    vk::DeviceSize{staging_buffers[1].get_mapped_memory().size()},
    vk::DeviceSize{staging_buffers[2].get_mapped_memory().size()},
  };

  auto const src_buffers = std::array{
    *staging_buffers[0].get_buffer().get(),
    *staging_buffers[1].get_buffer().get(),
    *staging_buffers[2].get_buffer().get(),
  };

  auto const & dst_buffers = self->transfer_buffers
                               .emplace_front(mesh_buffer.self->device_memory_resource->get_buffer_dependencies(),
                                              allocation_sizes)
                               .get_buffers()
                               .get();

  constexpr auto command_buffer_begin_info = vk::CommandBufferBeginInfo{}  //
                                               .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

  self->transfer_command_buffers.get().front().begin(command_buffer_begin_info);

  for(auto const [allocation_size, src_buffer, dst_buffer] : std::views::zip(allocation_sizes, src_buffers, dst_buffers))
  {
    auto const vertices_region = vk::BufferCopy2{}.setSize(allocation_size);

    auto const vertices_copy_buffer_info = vk::CopyBufferInfo2{}
                                             .setDstBuffer(src_buffer)
                                             .setSrcBuffer(dst_buffer)
                                             .setRegions(vertices_region);

    self->transfer_command_buffers.get().front().copyBuffer2(vertices_copy_buffer_info);
  };

  self->transfer_command_buffers.get().front().end();

  auto const command_buffer_submit_info = vk::CommandBufferSubmitInfo{}  //
                                            .setCommandBuffer(self->transfer_command_buffers.get().front());

  auto const submit_info = vk::SubmitInfo2{}.setCommandBufferInfos(command_buffer_submit_info);

  self->transfer_command_buffers.get_dependency<vk::raii::Queue>().submit2(submit_info,
                                                                           self->transfer_complete_fences.get().front());
}

void graphics::graphics_pipeline::attach(camera const &)
{
}

void graphics::graphics_pipeline::draw(render_window & render_window)
{
  self->render_complete_semaphores.get().splice(self->render_complete_semaphores.get().end(),
                                                self->render_complete_semaphores.get(),
                                                self->render_complete_semaphores.get().begin());

  self->present_complete_semaphores.get().splice(self->present_complete_semaphores.get().end(),
                                                 self->present_complete_semaphores.get(),
                                                 self->present_complete_semaphores.get().begin());

  self->draw_complete_fences.get().splice(self->draw_complete_fences.get().end(),
                                          self->draw_complete_fences.get(),
                                          self->draw_complete_fences.get().begin());

  self->draw_command_buffers.get().splice(self->draw_command_buffers.get().end(),
                                          self->draw_command_buffers.get(),
                                          self->draw_command_buffers.get().begin());

  wait_for_fences(self->draw_complete_fences.get_dependency<vk::raii::Device>(),
                  self->draw_complete_fences.get().front(),
                  self->transfer_complete_fences.get().front());

  if(self->transfer_buffers.size() > 1)
    for(std::size_t i = 0; i < self->transfer_buffers.size() - 1; ++i)
      self->transfer_buffers.pop_back();

  auto const [acquire_next_image_result, next_image_index] = render_window.self->swapchain.get().acquireNextImage(
    std::numeric_limits<std::uint64_t>::max(),
    self->present_complete_semaphores.get().front());

  if(acquire_next_image_result > vk::Result::eSuccess)
    logging::warning() << "acquiring the next image returned a warning: " << vk::to_string(acquire_next_image_result);

  constexpr auto image_subresource_range = vk::ImageSubresourceRange{}
                                             .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                             .setBaseMipLevel(0)
                                             .setLevelCount(1)
                                             .setBaseArrayLayer(0)
                                             .setLayerCount(1);

  auto const write_image_memory_barrier = vk::ImageMemoryBarrier2{}
                                            .setSrcStageMask(vk::PipelineStageFlagBits2::eNone)
                                            .setSrcAccessMask(vk::AccessFlagBits2::eNone)
                                            .setDstStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
                                            .setDstAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite)
                                            .setOldLayout(vk::ImageLayout::eUndefined)
                                            .setNewLayout(vk::ImageLayout::eColorAttachmentOptimal)
                                            .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                                            .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
                                            .setImage(render_window.self->images[next_image_index])
                                            .setSubresourceRange(image_subresource_range);

  auto const write_dependency_info = vk::DependencyInfo{}              //
                                       .setImageMemoryBarrierCount(1)  //
                                       .setPImageMemoryBarriers(&write_image_memory_barrier);

  auto const attachmentInfo = vk::RenderingAttachmentInfo{}
                                .setImageView(render_window.self->image_views.get()[next_image_index])
                                .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
                                .setLoadOp(vk::AttachmentLoadOp::eClear)
                                .setStoreOp(vk::AttachmentStoreOp::eStore)
                                .setClearValue(vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f));

  auto const viewport = vk::Viewport{}
                          .setX(0.0f)
                          .setY(0.0f)
                          .setWidth(static_cast<float>(render_window.self->swapchain_create_info.get().imageExtent.width))
                          .setHeight(static_cast<float>(render_window.self->swapchain_create_info.get().imageExtent.height))
                          .setMinDepth(0.0f)
                          .setMaxDepth(1.0f);

  auto const scissor = vk::Rect2D{}
                         .setOffset(vk::Offset2D{}.setX(0).setY(0))
                         .setExtent(render_window.self->swapchain_create_info.get().imageExtent);

  auto const rendering_info = vk::RenderingInfo{}
                                .setRenderArea(scissor)
                                .setLayerCount(1)
                                .setColorAttachmentCount(1)
                                .setPColorAttachments(&attachmentInfo);

  auto const present_image_memory_barrier = vk::ImageMemoryBarrier2{}
                                              .setSrcStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
                                              .setSrcAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite)
                                              .setDstStageMask(vk::PipelineStageFlagBits2::eNone)
                                              .setDstAccessMask(vk::AccessFlagBits2::eNone)
                                              .setOldLayout(vk::ImageLayout::eColorAttachmentOptimal)
                                              .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
                                              .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                                              .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
                                              .setImage(render_window.self->images[next_image_index])
                                              .setSubresourceRange(image_subresource_range);

  auto const present_dependency_info = vk::DependencyInfo{}              //
                                         .setImageMemoryBarrierCount(1)  //
                                         .setPImageMemoryBarriers(&present_image_memory_barrier);

  self->draw_command_buffers.get().front().reset();
  self->draw_command_buffers.get().front().begin({});
  self->draw_command_buffers.get().front().pipelineBarrier2(write_dependency_info);
  self->draw_command_buffers.get().front().beginRendering(rendering_info);
  self->draw_command_buffers.get().front().bindPipeline(vk::PipelineBindPoint::eGraphics, self->pipeline.get());
  self->draw_command_buffers.get().front().setViewport(0, viewport);
  self->draw_command_buffers.get().front().setScissor(0, scissor);
  self->draw_command_buffers.get().front().drawMeshTasksEXT(1, 1, 1);
  self->draw_command_buffers.get().front().endRendering2KHR(vk::RenderingEndInfoKHR{});
  self->draw_command_buffers.get().front().pipelineBarrier2(present_dependency_info);
  self->draw_command_buffers.get().front().end();

  auto const wait_semaphore_submit_info = vk::SemaphoreSubmitInfo{}
                                            .setSemaphore(self->present_complete_semaphores.get().front())
                                            .setStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput);

  auto const signal_semaphore_submit_info = vk::SemaphoreSubmitInfo{}
                                              .setSemaphore(self->render_complete_semaphores.get().front())
                                              .setStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput);

  auto const command_buffer_submit_info = vk::CommandBufferSubmitInfo{}  //
                                            .setCommandBuffer(self->draw_command_buffers.get().front());

  auto const submit_info = vk::SubmitInfo2{}
                             .setWaitSemaphoreInfos(wait_semaphore_submit_info)
                             .setCommandBufferInfos(command_buffer_submit_info)
                             .setSignalSemaphoreInfos(signal_semaphore_submit_info);

  self->draw_command_buffers.get_dependency<vk::raii::Queue>().submit2(submit_info, self->draw_complete_fences.get().front());

  auto const present_info = vk::PresentInfoKHR{}
                              .setWaitSemaphores(*self->render_complete_semaphores.get().front())
                              .setSwapchains(*render_window.self->swapchain.get())
                              .setImageIndices(next_image_index);

  auto const present_result = self->draw_command_buffers.get_dependency<vk::raii::Queue>().presentKHR(present_info);

  if(present_result > vk::Result::eSuccess)
    logging::warning() << "presenting returned a warning: " << vk::to_string(present_result);
}
