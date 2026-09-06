#include "impl.hpp"

#include <khronos/graphical_device.hpp>
#include <khronos/graphics_pipeline.hpp>

extern unsigned int  shader_spv_len;
extern unsigned char shader_spv[];

namespace
{
[[nodiscard]]
static vk::raii::ShaderModule create_shader_module(vk::raii::Device const & device,
                                                   unsigned int const       spv_len,
                                                   unsigned char const      spv[])
{
  auto const shader_binary = std::make_unique_for_overwrite<std::uint32_t[]>(spv_len);

  std::memcpy(shader_binary.get(), spv, spv_len);

  auto const shader_module_create_info = vk::ShaderModuleCreateInfo{}.setCodeSize(spv_len).setPCode(shader_binary.get());

  return {device, shader_module_create_info};
}

[[nodiscard]]
static vk::raii::PipelineLayout create_pipeline_layout(vk::raii::Device const & device)
{
  auto const pipeline_layout_info = vk::PipelineLayoutCreateInfo{};

  return {device, pipeline_layout_info};
}

[[nodiscard]]
static vk::raii::Pipeline create_pipeline(vk::raii::Device const &         device,
                                          vk::raii::PipelineLayout const & pipeline_layout,
                                          vk::raii::ShaderModule const &   shader_module)
{
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
      .setOffset(offsetof(khronos::graphics_pipeline::vertex, pos)),
    vk::VertexInputAttributeDescription{}
      .setLocation(1)
      .setBinding(0)
      .setFormat(vk::Format::eR32G32B32Sfloat)
      .setOffset(offsetof(khronos::graphics_pipeline::vertex, colour)),
  };

  auto const vertex_input_binding_description = vk::VertexInputBindingDescription{}.setBinding(0).setInputRate(
    vk::VertexInputRate::eVertex);

  auto const pipeline_vertex_input_state_create_info = vk::PipelineVertexInputStateCreateInfo{}
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

  constexpr auto dynamic_states = std::array{vk::DynamicState::eViewport,
                                             vk::DynamicState::eScissor,
                                             vk::DynamicState::eVertexInputBindingStride};

  auto const pipeline_dynamic_state_create_info = vk::PipelineDynamicStateCreateInfo{}.setDynamicStates(dynamic_states);

  constexpr auto formats = vk::Format::eB8G8R8A8Srgb;

  auto const graphics_pipeline_create_info = vk::StructureChain{
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
    vk::PipelineRenderingCreateInfo{}  //
      .setColorAttachmentFormats(formats)};

  return {device, nullptr, graphics_pipeline_create_info.get()};
}

[[nodiscard]]
static khronos::graphics_pipeline_impl create_graphics_pipeline_impl(khronos::device const & device)
{
  auto pipeline_layout = khronos::pipeline_layout(device.as_dependencies(), create_pipeline_layout(device.get()));

  auto pipeline = khronos::pipeline(
    pipeline_layout.as_dependencies(),
    create_pipeline(device.get(), pipeline_layout.get(), create_shader_module(device.get(), shader_spv_len, shader_spv)));

  return {std::move(pipeline_layout), std::move(pipeline)};
}
}

khronos::graphics_pipeline::graphics_pipeline(graphical_device const & graphical_device)
: self(std::make_unique<graphics_pipeline_impl>(create_graphics_pipeline_impl(graphical_device.self->device)))
{
}

khronos::graphics_pipeline::~graphics_pipeline() noexcept = default;
