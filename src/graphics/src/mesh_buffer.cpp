#include "device_memory_resource.hpp"
#include "find_memory_type_index.hpp"
#include "graphical_device_impl.hpp"
#include "mesh_buffer_impl.hpp"

#include <graphics/graphical_device.hpp>
#include <graphics/graphics_pipeline.hpp>
#include <graphics/mesh_buffer.hpp>
#include <logging/logging.hpp>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <cstddef>
#include <cstring>
#include <stdexcept>

namespace
{
static void wait_for_fence(vk::raii::Device const & device, vk::raii::Fence const & fence)
{
  auto const wait_for_fences_result = device.waitForFences(*fence, vk::True, std::numeric_limits<std::uint64_t>::max());

  if(wait_for_fences_result < vk::Result::eSuccess)
    logging::error() << "wait for fences returned a error: " << vk::to_string(wait_for_fences_result);

  if(wait_for_fences_result > vk::Result::eSuccess)
    logging::warning() << "wait for fences returned a warning: " << vk::to_string(wait_for_fences_result);

  device.resetFences(*fence);
}

static void wait_for_fence(graphics::vulkan::fence const & fence)
{
  wait_for_fence(fence.get_dependency<vk::raii::Device>(), fence);
}

static void wait_for_fence_no_throw(vk::raii::Device const & device, vk::raii::Fence const & fence) noexcept
{
  try
  {
    wait_for_fence(device, fence);
  }
  catch(...)
  {
  }
}

[[nodiscard]]
static graphics::mesh_buffer_impl create_mesh_buffer_impl(graphics::vulkan::queue const & queue,
                                                          std::uint32_t const             queue_family_index)
{
  auto resource = std::make_unique<graphics::device_memory_resource>(queue.get_dependencies(),
                                                                     vk::BufferUsageFlagBits::eTransferSrc);

  constexpr auto fence_create_info = vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled);

  auto transfer_complete = graphics::vulkan::fence{wait_for_fence_no_throw,
                                                   queue.as_dependencies(),
                                                   queue.get_dependency<vk::raii::Device>(),
                                                   fence_create_info};

  auto const command_pool_create_info = vk::CommandPoolCreateInfo{}
                                          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                                          .setQueueFamilyIndex(queue_family_index);

  auto transfer_command_pool = graphics::vulkan::command_pool{transfer_complete.get_dependencies(),
                                                              transfer_complete.get_dependency<vk::raii::Device>(),
                                                              command_pool_create_info};

  auto const command_buffer_allocate_info = vk::CommandBufferAllocateInfo{}
                                              .setCommandPool(transfer_command_pool.get())
                                              .setLevel(vk::CommandBufferLevel::ePrimary)
                                              .setCommandBufferCount(1);

  auto command_buffers = vk::raii::CommandBuffers{transfer_command_pool.get_dependency<vk::raii::Device>(),
                                                  command_buffer_allocate_info};

  auto transfer_command_buffer = graphics::vulkan::command_buffer{
    wait_for_fence_no_throw,
    graphics::dependency_union(transfer_command_pool.as_dependencies(), transfer_complete.as_dependencies()),
    std::move(command_buffers.front())};

  auto buffers_in_use = graphics::vulkan::fences{wait_for_fence_no_throw, queue.as_dependencies()};

  auto * const resource_ptr = resource.get();

  auto device_local_buffers = graphics::vulkan::buffers{wait_for_fence_no_throw, buffers_in_use.as_dependencies()};

  auto device_local_device_memories = graphics::vulkan::device_memories{wait_for_fence_no_throw,
                                                                        device_local_buffers.as_dependencies()};

  return {std::move(resource),
          std::pmr::vector<graphics::mesh_buffer::vertex>{resource_ptr},
          std::pmr::vector<std::uint32_t>{resource_ptr},
          std::pmr::vector<std::byte>{resource_ptr},
          {},
          {},
          std::move(buffers_in_use),
          std::move(device_local_buffers),
          std::move(device_local_device_memories),
          {},
          std::move(transfer_complete),
          std::move(transfer_command_pool),
          std::move(transfer_command_buffer)};
}
}

graphics::mesh_buffer::mesh_buffer(graphical_device const & graphical_device)
: self(std::make_unique<mesh_buffer_impl>(
    create_mesh_buffer_impl(graphical_device.self->queue, graphical_device.self->queue_family_index)))
{
}

graphics::mesh_buffer::~mesh_buffer() noexcept = default;

void graphics::mesh_buffer::push_back(mesh const & mesh)
{
  if(self->id_to_mesh.contains(mesh.id))
    throw std::logic_error("");

  self->meshes.push_back({.id             = mesh.id,
                          .vertices_begin = self->vertex_buffer.size(),
                          .vertices_end   = self->vertex_buffer.size() + mesh.vertices.size(),
                          .indices_begin  = self->index_buffer.size(),
                          .indices_end    = self->index_buffer.size() + mesh.indices.size(),
                          .textures_begin = self->texture_buffer.size(),
                          .textures_end   = self->texture_buffer.size() + mesh.texture.size()});

  self->id_to_mesh.emplace_hint(self->id_to_mesh.end(), mesh.id, self->meshes.size() - 1);
  self->vertex_buffer.append_range(mesh.vertices);
  self->index_buffer.append_range(mesh.indices);
  self->texture_buffer.append_range(mesh.texture);
}

void graphics::mesh_buffer::pop_back() noexcept
{
  self->vertex_buffer.resize(self->meshes.back().vertices_begin);
  self->index_buffer.resize(self->meshes.back().indices_begin);
  self->texture_buffer.resize(self->meshes.back().textures_begin);
  self->id_to_mesh.erase(self->meshes.back().id);
  self->meshes.pop_back();
}

void graphics::mesh_buffer::clear() noexcept
{
  self->vertex_buffer.clear();
  self->index_buffer.clear();
  self->texture_buffer.clear();
  self->id_to_mesh.clear();
  self->meshes.clear();
}

std::size_t graphics::mesh_buffer::size() const noexcept
{
  return self->meshes.size();
}

std::size_t graphics::mesh_buffer::byte_size() const noexcept
{
  return std::span{self->vertex_buffer}.size_bytes() + std::span{self->index_buffer}.size_bytes()
       + std::span{self->texture_buffer}.size_bytes();
}

bool graphics::mesh_buffer::empty() const noexcept
{
  return self->meshes.empty();
}

void graphics::mesh_buffer::reserve_vertices(std::size_t const size)
{
  self->vertex_buffer.reserve(size);
}

void graphics::mesh_buffer::reserve_indices(std::size_t const size)
{
  self->index_buffer.reserve(size);
}

void graphics::mesh_buffer::reserve_texture(std::size_t const size)
{
  self->texture_buffer.reserve(size);
}

graphics::mesh_buffer::mesh_view graphics::mesh_buffer::operator[](std::size_t const index) noexcept
{
  return {
    self->meshes[index].id,
    std::span(std::ranges::next(self->vertex_buffer.data(), static_cast<std::ptrdiff_t>(self->meshes[index].vertices_begin)),
              self->meshes[index].vertices_end),
    std::span(std::ranges::next(self->index_buffer.begin(), static_cast<std::ptrdiff_t>(self->meshes[index].indices_begin)),
              self->meshes[index].indices_end),
    std::span(std::ranges::next(self->texture_buffer.begin(), static_cast<std::ptrdiff_t>(self->meshes[index].textures_begin)),
              self->meshes[index].textures_end),
  };
}

graphics::mesh_buffer::const_mesh_view graphics::mesh_buffer::operator[](std::size_t const index) const noexcept
{
  return {
    self->meshes[index].id,
    std::span(std::ranges::next(self->vertex_buffer.data(), static_cast<std::ptrdiff_t>(self->meshes[index].vertices_begin)),
              self->meshes[index].vertices_end),
    std::span(std::ranges::next(self->index_buffer.begin(), static_cast<std::ptrdiff_t>(self->meshes[index].indices_begin)),
              self->meshes[index].indices_end),
    std::span(std::ranges::next(self->texture_buffer.begin(), static_cast<std::ptrdiff_t>(self->meshes[index].textures_begin)),
              self->meshes[index].textures_end),
  };
}

graphics::mesh_buffer::mesh_view graphics::mesh_buffer::at(std::size_t const index)
{
  if(index >= size())
    throw std::out_of_range("graphics::mesh_buffer::at");

  return (*this)[index];
}

graphics::mesh_buffer::const_mesh_view graphics::mesh_buffer::at(std::size_t const index) const
{
  if(index >= size())
    throw std::out_of_range("graphics::mesh_buffer::at");

  return (*this)[index];
}

graphics::mesh_buffer::mesh_view graphics::mesh_buffer::find(std::uint32_t const id)
{
  auto mesh_index = self->id_to_mesh.find(id);

  if(mesh_index == self->id_to_mesh.end())
    throw std::out_of_range("graphics::mesh_buffer::find");

  return (*this)[mesh_index->second];
}

graphics::mesh_buffer::const_mesh_view graphics::mesh_buffer::find(std::uint32_t const id) const
{
  auto mesh_index = self->id_to_mesh.find(id);

  if(mesh_index == self->id_to_mesh.end())
    throw std::out_of_range("graphics::mesh_buffer::find");

  return (*this)[mesh_index->second];
}

void graphics::mesh_buffer::flush()
{
  if(self->buffer_sizes.empty() or self->buffer_sizes.front() < byte_size())
  {
    auto const usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer
                     | vk::BufferUsageFlagBits::eVertexBuffer;

    auto const buffer_create_info = vk::BufferCreateInfo{}   //
                                      .setSize(byte_size())  //
                                      .setUsage(usage)       //
                                      .setSharingMode(vk::SharingMode::eExclusive);

    self->device_local_buffers.get().emplace_front(self->device_local_buffers.get_dependency<vk::raii::Device>(),
                                                   buffer_create_info);

    auto const memory_properties = self->resource->get_buffer_dependencies()
                                     .template get<vk::raii::PhysicalDevice>()
                                     .getMemoryProperties();

    auto const memory_requirements = self->device_local_buffers.get().front().getMemoryRequirements();

    auto const memory_property = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

    auto const memory_type_index = find_memory_type_index(memory_requirements, memory_properties, memory_property);

    if(not memory_type_index)
      throw std::bad_alloc();

    auto const memory_allocate_info = vk::MemoryAllocateInfo{}                   //
                                        .setMemoryTypeIndex(*memory_type_index)  //
                                        .setAllocationSize(memory_requirements.size);

    self->device_local_device_memories.get().emplace_front(
      self->device_local_device_memories.get_dependency<vk::raii::Device>(),
      memory_allocate_info);

    self->device_local_buffers.get().front().bindMemory(self->device_local_device_memories.get().front(), 0);

    self->buffer_sizes.emplace_front(buffer_create_info.size);

    constexpr auto fence_create_info = vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled);

    self->buffer_in_use.get().emplace_front(self->buffer_in_use.get_dependency<vk::raii::Device>(), fence_create_info);
  }
  else
  {
    wait_for_fence(self->buffer_in_use.get_dependency<vk::raii::Device>(), self->buffer_in_use.get().front());
  }

  auto const command_buffer_begin_info = vk::CommandBufferBeginInfo{}.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

  auto const vertices_region = vk::BufferCopy2{}   //
                                 .setSrcOffset(0)  //
                                 .setDstOffset(0)  //
                                 .setSize(std::span{self->vertex_buffer}.size_bytes());

  auto const vertices_copy_buffer_info = vk::CopyBufferInfo2{}
                                           .setDstBuffer(*self->device_local_buffers.get().front())
                                           .setSrcBuffer(self->resource->get_buffer(self->vertex_buffer.data())->get())
                                           .setRegions(vertices_region);

  auto const indices_region = vk::BufferCopy2{}
                                .setSrcOffset(vertices_region.srcOffset + vertices_region.size)
                                .setDstOffset(vertices_region.dstOffset + vertices_region.size)
                                .setSize(std::span{self->index_buffer}.size_bytes());

  auto const indices_copy_buffer_info = vk::CopyBufferInfo2{}
                                          .setDstBuffer(self->device_local_buffers.get().front())
                                          .setSrcBuffer(self->resource->get_buffer(self->index_buffer.data())->get())
                                          .setRegions(indices_region);

  auto const textures_region = vk::BufferCopy2{}
                                 .setSrcOffset(indices_region.srcOffset + indices_region.size)
                                 .setDstOffset(indices_region.dstOffset + indices_region.size)
                                 .setSize(std::span{self->texture_buffer}.size_bytes());

  auto const textures_copy_buffer_info = vk::CopyBufferInfo2{}
                                           .setDstBuffer(self->device_local_buffers.get().front())
                                           .setSrcBuffer(self->resource->get_buffer(self->texture_buffer.data())->get())
                                           .setRegions(textures_region);

  wait_for_fence(self->transfer_complete);

  self->transfer_command_buffer.get().begin(command_buffer_begin_info);
  self->transfer_command_buffer.get().copyBuffer2(vertices_copy_buffer_info);
  self->transfer_command_buffer.get().copyBuffer2(indices_copy_buffer_info);
  self->transfer_command_buffer.get().copyBuffer2(textures_copy_buffer_info);
  self->transfer_command_buffer.get().end();

  auto const submit_info = vk::SubmitInfo{}.setCommandBuffers(self->transfer_command_buffer.get());

  self->transfer_command_buffer.get_dependency<vk::raii::Queue>().submit(submit_info, self->transfer_complete.get());

  wait_for_fence(self->transfer_complete);
}