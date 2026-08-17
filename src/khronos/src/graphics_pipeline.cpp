#include "detail/make_shared_with_data.hpp"

#include <khronos/graphics_pipeline.hpp>
#include <khronos/memory_buffer.hpp>

extern unsigned int  shader_spv_len;
extern unsigned char shader_spv[];

khronos::graphics_pipeline::graphics_pipeline(
  std::shared_ptr<vk::raii::Device const> const &           device,
  std::shared_ptr<vk::SwapchainCreateInfoKHR const> const & default_swapchain_create_info)
{
  assert(shader_spv_len % sizeof(std::uint32_t) == 0);

  auto const code = std::make_unique_for_overwrite<std::uint32_t[]>(shader_spv_len);

  std::memcpy(code.get(), shader_spv, shader_spv_len);

  auto const shader_module_create_info = vk::ShaderModuleCreateInfo{}.setCodeSize(shader_spv_len).setPCode(code.get());

  auto const shader_module = device->createShaderModule(shader_module_create_info);

  auto const pipeline_shader_stage_create_info = std::array{
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eVertex)
      .setModule(shader_module)
      .setPName("vert_main"),
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eFragment)
      .setModule(shader_module)
      .setPName("frag_main"),
  };

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

  constexpr auto pipeline_input_assembly_state_create_info = vk::PipelineInputAssemblyStateCreateInfo{}
                                                               .setTopology(vk::PrimitiveTopology::eTriangleList)
                                                               .setPrimitiveRestartEnable(vk::False);

  constexpr auto pipeline_viewport_state_create_info = vk::PipelineViewportStateCreateInfo{}  //
                                                         .setViewportCount(1)
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

  auto const pipeline_layout_info = vk::PipelineLayoutCreateInfo{};

  pipeline_layout = detail::make_shared_with_data<vk::raii::PipelineLayout const>(*device, pipeline_layout_info);

  detail::emplace_data(pipeline_layout, device);

  auto const & [graphics_pipeline_create_info, _]
    = vk::StructureChain{vk::GraphicsPipelineCreateInfo{}
                           .setStages(pipeline_shader_stage_create_info)
                           .setPVertexInputState(&pipeline_vertex_input_state_create_info)
                           .setPInputAssemblyState(&pipeline_input_assembly_state_create_info)
                           .setPViewportState(&pipeline_viewport_state_create_info)
                           .setPRasterizationState(&pipeline_rasterization_state_create_info)
                           .setPMultisampleState(&pipeline_multisample_state_create_info)
                           .setPColorBlendState(&pipeline_color_blend_state_create_info)
                           .setPDynamicState(&pipeline_dynamic_state_create_info)
                           .setLayout(*pipeline_layout),
                         vk::PipelineRenderingCreateInfo{}  //
                           .setColorAttachmentFormats(default_swapchain_create_info->imageFormat)};

  pipeline = detail::make_shared_with_data<vk::raii::Pipeline const>(*device, nullptr, graphics_pipeline_create_info);

  detail::emplace_data(pipeline, pipeline_layout);
}