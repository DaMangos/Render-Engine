#include "graphical_device_impl.hpp"
#include "graphics_pipeline_impl.hpp"

#include <graphics/graphical_device.hpp>
#include <graphics/graphics_pipeline.hpp>

extern unsigned int  task_spv_len;
extern unsigned char task_spv[];

extern unsigned int  mesh_spv_len;
extern unsigned char mesh_spv[];

extern unsigned int  fragment_spv_len;
extern unsigned char fragment_spv[];

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
static vk::raii::Pipeline create_pipeline(vk::raii::Device const & device, vk::raii::PipelineLayout const & pipeline_layout)
{
  auto const pipeline_shader_stage_create_info = std::array{
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eTaskEXT)
      .setModule(create_shader_module(device, task_spv_len, task_spv))
      .setPName("main"),
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eMeshEXT)
      .setModule(create_shader_module(device, mesh_spv_len, mesh_spv))
      .setPName("main"),
    vk::PipelineShaderStageCreateInfo{}
      .setStage(vk::ShaderStageFlagBits::eFragment)
      .setModule(create_shader_module(device, fragment_spv_len, fragment_spv))
      .setPName("main"),
  };

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

  constexpr auto dynamic_states = std::array{
    vk::DynamicState::eViewport,
    vk::DynamicState::eScissor,
    vk::DynamicState::eVertexInputBindingStride,
  };

  auto const pipeline_dynamic_state_create_info = vk::PipelineDynamicStateCreateInfo{}.setDynamicStates(dynamic_states);

  constexpr auto formats = vk::Format::eB8G8R8A8Srgb;

  auto const graphics_pipeline_create_info = vk::StructureChain{
    vk::GraphicsPipelineCreateInfo{}
      .setStages(pipeline_shader_stage_create_info)
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
static graphics::graphics_pipeline_impl create_graphics_pipeline_impl(graphics::vulkan::device const & device)
{
  auto pipeline_layout = graphics::vulkan::pipeline_layout(device.as_dependencies(), create_pipeline_layout(device.get()));

  auto pipeline = graphics::vulkan::pipeline(pipeline_layout.as_dependencies(),
                                             create_pipeline(device.get(), pipeline_layout.get()));

  return {std::move(pipeline_layout), std::move(pipeline)};
}
}

graphics::graphics_pipeline::graphics_pipeline(graphical_device const & graphical_device)
: self(std::make_unique<graphics_pipeline_impl>(create_graphics_pipeline_impl(graphical_device.self->device)))
{
}

graphics::graphics_pipeline::~graphics_pipeline() noexcept = default;
