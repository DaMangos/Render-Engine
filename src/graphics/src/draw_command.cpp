#include "draw_command_impl.hpp"

#include <graphics/draw_command.hpp>
#include <graphics/graphical_device.hpp>
#include <graphics/graphics_pipeline.hpp>
#include <graphics/render_window.hpp>
#include <logging/logging.hpp>

namespace
{
[[nodiscard]]
static std::uint32_t acquire_next_image_index(vk::raii::SwapchainKHR const & swapchain,
                                              vk::raii::Semaphore const &    present_complete_semaphores)
{
  auto const & [acquire_next_image_result, next_image_index] = swapchain.acquireNextImage(
    std::numeric_limits<std::uint64_t>::max(),
    *present_complete_semaphores);

  if(acquire_next_image_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(acquire_next_image_result), "failed to acquire next image"};

  if(acquire_next_image_result > vk::Result::eSuccess)
    logging::warning() << "acquiring the next image returned a warning: " << vk::to_string(acquire_next_image_result);

  return next_image_index;
}
}

graphics::draw_command::draw_command(graphical_device const & graphical_device,
                                     render_window const &    render_window,
                                     std::size_t const        command_buffer_count)
: self(std::make_unique<draw_command_impl>(create_draw_command_impl(graphical_device.self->queue,
                                                                    graphical_device.self->queue_family_index,
                                                                    render_window.self->images.size(),
                                                                    command_buffer_count)))
{
}

graphics::draw_command::~draw_command() noexcept = default;

void graphics::draw_command::operator()(graphics_pipeline & graphics_pipeline,
                                        transfer_buffer &   transfer_buffer,
                                        render_window &     render_window)
{
  auto const & device = self->fences.get_dependency<vk::raii::Device>();
  auto const & queue  = self->fences.get_dependency<vk::raii::Queue>();

  auto const & fence                       = get_next(self->fences.get());
  auto const & present_complete_semaphores = get_next(self->present_complete_semaphores.get());

  auto const & pipeline = graphics_pipeline.self->pipeline.get();

  auto const & swapchain_create_info = render_window.self->swapchain_create_info.get();
  auto const & swapchain             = render_window.self->swapchain.get();

  auto const & index_buffer_region = transfer_buffer.self->usage_to_occupied_region.at(vk::BufferUsageFlagBits::eIndexBuffer);

  auto const & vertex_buffer_region = transfer_buffer.self->usage_to_occupied_region.at(vk::BufferUsageFlagBits::eVertexBuffer);

  auto const & buffer = transfer_buffer.self->buffer.get();

  wait_for_fence(device, fence);

  auto const next_image_index = acquire_next_image_index(swapchain, present_complete_semaphores);

  auto const & render_complete_semaphores = get_next(self->render_complete_semaphores.get());
  auto const & command_buffer             = get_next(self->command_buffers.get());

  auto const & image      = render_window.self->images.at(next_image_index);
  auto const & image_view = render_window.self->image_views.get().at(next_image_index);

  auto const image_subresource_range = vk::ImageSubresourceRange{}
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
                                            .setImage(image)
                                            .setSubresourceRange(image_subresource_range);

  auto const write_dependency_info = vk::DependencyInfo{}              //
                                       .setImageMemoryBarrierCount(1)  //
                                       .setPImageMemoryBarriers(&write_image_memory_barrier);

  auto const attachmentInfo = vk::RenderingAttachmentInfo{}
                                .setImageView(image_view)
                                .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
                                .setLoadOp(vk::AttachmentLoadOp::eClear)
                                .setStoreOp(vk::AttachmentStoreOp::eStore)
                                .setClearValue(vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f));

  auto const viewport = vk::Viewport{}
                          .setX(0.0f)
                          .setY(0.0f)
                          .setWidth(static_cast<float>(swapchain_create_info.imageExtent.width))
                          .setHeight(static_cast<float>(swapchain_create_info.imageExtent.height))
                          .setMinDepth(0.0f)
                          .setMaxDepth(1.0f);

  auto const scissor = vk::Rect2D{}.setOffset(vk::Offset2D{}.setX(0).setY(0)).setExtent(swapchain_create_info.imageExtent);

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
                                              .setImage(image)
                                              .setSubresourceRange(image_subresource_range);

  auto const present_dependency_info = vk::DependencyInfo{}              //
                                         .setImageMemoryBarrierCount(1)  //
                                         .setPImageMemoryBarriers(&present_image_memory_barrier);

  auto const wait_destination_stage_mask = vk::PipelineStageFlags{vk::PipelineStageFlagBits::eColorAttachmentOutput};

  command_buffer.reset();
  command_buffer.begin({});
  command_buffer.pipelineBarrier2(write_dependency_info);
  command_buffer.beginRendering(rendering_info);
  command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
  command_buffer.setViewport(0, viewport);
  command_buffer.setScissor(0, scissor);
  command_buffer.drawMeshTasksEXT(1, 1, 1);
  command_buffer.endRendering();
  command_buffer.pipelineBarrier2(present_dependency_info);
  command_buffer.end();

  auto const graphics_submit_info = vk::SubmitInfo{}
                                      .setWaitSemaphores(*present_complete_semaphores)
                                      .setWaitDstStageMask(wait_destination_stage_mask)
                                      .setCommandBuffers(*command_buffer)
                                      .setSignalSemaphores(*render_complete_semaphores);

  queue.submit(graphics_submit_info, *fence);

  auto const present_info = vk::PresentInfoKHR{}
                              .setWaitSemaphores(*render_complete_semaphores)
                              .setSwapchains(*swapchain)
                              .setImageIndices(next_image_index);

  auto const present_result = queue.presentKHR(present_info);

  if(present_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(present_result), "failed to present"};

  if(present_result > vk::Result::eSuccess)
    logging::warning() << "presenting returned a warning: " << vk::to_string(present_result);
}
