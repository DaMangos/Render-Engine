#include "command.hpp"

#include <khronos/graphical_device.hpp>
#include <khronos/transfer_command.hpp>
#include <logging/logging.hpp>

#include <ranges>
#include <vector>

namespace
{
[[nodiscard]]
static vk::raii::CommandPool create_command_pool(vk::raii::Device const & device, std::uint32_t const queue_family_index)
{
  auto const command_pool_create_info = vk::CommandPoolCreateInfo{}
                                          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                                          .setQueueFamilyIndex(queue_family_index);

  return {device, command_pool_create_info};
}

[[nodiscard]]
static std::list<vk::raii::CommandBuffer> create_command_buffers(vk::raii::Device const &      device,
                                                                 vk::raii::CommandPool const & command_pool,
                                                                 std::size_t const             count)
{
  auto const command_buffer_allocate_info = vk::CommandBufferAllocateInfo{}
                                              .setCommandPool(*command_pool)
                                              .setLevel(vk::CommandBufferLevel::ePrimary)
                                              .setCommandBufferCount(static_cast<std::uint32_t>(count));

  auto command_buffer = vk::raii::CommandBuffers{device, command_buffer_allocate_info};

  return command_buffer | std::views::as_rvalue | std::ranges::to<std::list>();
}

[[nodiscard]]
static std::list<vk::raii::Fence> create_fences(vk::raii::Device const & device, std::size_t const count)
{
  std::list<vk::raii::Fence> fences;

  auto const fence_create_info = vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled);

  for(std::size_t i = 0; i < count; i++)
    fences.emplace_back(device, fence_create_info);

  return fences;
}

[[nodiscard]]
static std::list<vk::raii::Semaphore> create_semaphores(vk::raii::Device const & device, std::size_t const count)
{
  std::list<vk::raii::Semaphore> semaphores;

  for(std::size_t i = 0; i < count; i++)
    semaphores.emplace_back(device, vk::SemaphoreCreateInfo{});

  return semaphores;
}

[[nodiscard]]
static std::vector<vk::Fence> to_underlying_fences(std::list<vk::raii::Fence> const & fences)
{
  return fences | std::views::transform([](auto const & fence) { return *fence; }) | std::ranges::to<std::vector>();
}

static void wait_for_fences_no_throw(std::list<vk::raii::CommandBuffer> const &,
                                     vk::raii::Device const &           device,
                                     std::list<vk::raii::Fence> const & fences) noexcept
{
  try
  {
    auto const underlying_fences = to_underlying_fences(fences);

    auto const wait_for_fences_result = device.waitForFences(underlying_fences,
                                                             vk::True,
                                                             std::numeric_limits<std::uint64_t>::max());

    if(wait_for_fences_result < vk::Result::eSuccess)
      logging::error() << "wait for fences returned a error: " << vk::to_string(wait_for_fences_result);

    if(wait_for_fences_result > vk::Result::eSuccess)
      logging::warning() << "wait for fences returned a warning: " << vk::to_string(wait_for_fences_result);

    device.resetFences(underlying_fences);
  }
  catch(std::exception const & error)
  {
    logging::error() << error.what();
  }
}

static void wait_for_queue_no_throw(std::list<vk::raii::Semaphore> const &, vk::raii::Queue const & queue) noexcept
{
  try
  {
    queue.waitIdle();
  }
  catch(std::exception const & error)
  {
    logging::error() << error.what();
  }
}

}

khronos::transfer_command_impl khronos::create_transfer_command_impl(queue const &       queue,
                                                                     std::uint32_t const queue_family_index,
                                                                     std::size_t const   command_buffer_count)
{
  auto fences = khronos::fences{queue.as_dependencies(),
                                create_fences(queue.template get_dependency<vk::raii::Device>(), command_buffer_count)};

  auto command_pool = khronos::command_pool{
    fences.as_dependencies(),
    create_command_pool(queue.template get_dependency<vk::raii::Device>(), queue_family_index)};

  auto command_buffers = khronos::command_buffers{
    wait_for_fences_no_throw,
    command_pool.as_dependencies(),
    create_command_buffers(queue.template get_dependency<vk::raii::Device>(), command_pool, command_buffer_count)};

  return {std::move(fences), std::move(command_pool), std::move(command_buffers)};
}

khronos::draw_command_impl khronos::create_draw_command_impl(queue const &       queue,
                                                             std::uint32_t const queue_family_index,
                                                             std::size_t const   image_count,
                                                             std::size_t const   command_buffer_count)
{
  auto fences = khronos::fences{queue.as_dependencies(),
                                create_fences(queue.template get_dependency<vk::raii::Device>(), command_buffer_count)};

  auto render_complete_semaphores = khronos::semaphores{
    wait_for_queue_no_throw,
    queue.as_dependencies(),
    create_semaphores(queue.template get_dependency<vk::raii::Device>(), image_count)};

  auto present_complete_semaphores = khronos::semaphores{
    wait_for_queue_no_throw,
    queue.as_dependencies(),
    create_semaphores(queue.template get_dependency<vk::raii::Device>(), command_buffer_count)};

  auto command_pool = khronos::command_pool{
    fences.as_dependencies(),
    create_command_pool(queue.template get_dependency<vk::raii::Device>(), queue_family_index)};

  auto command_buffers = khronos::command_buffers{
    wait_for_fences_no_throw,
    command_pool.as_dependencies(),
    create_command_buffers(queue.template get_dependency<vk::raii::Device>(), command_pool, command_buffer_count)};

  return {std::move(fences),
          std::move(render_complete_semaphores),
          std::move(present_complete_semaphores),
          std::move(command_pool),
          std::move(command_buffers)};
}

void khronos::wait_for_fence(vk::raii::Device const & device, vk::raii::Fence const & fence)
{
  auto const wait_for_fences_result = device.waitForFences(*fence, vk::True, std::numeric_limits<std::uint64_t>::max());

  if(wait_for_fences_result < vk::Result::eSuccess)
    throw vk::SystemError{vk::make_error_code(wait_for_fences_result), "failed to wait for fence"};

  if(wait_for_fences_result > vk::Result::eSuccess)
    logging::warning() << "wait for fence returned a warning: " << vk::to_string(wait_for_fences_result);

  device.resetFences(*fence);
}
