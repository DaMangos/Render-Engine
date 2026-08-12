#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <expected>
#include <memory>
#include <vector>

namespace khronos::detail
{
[[nodiscard]]
std::expected<std::vector<char const *>, std::vector<char const *>> get_required_device_extensions(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device);

inline auto const
  required_physical_device_features = vk::StructureChain{vk::PhysicalDeviceFeatures2{},
                                                         vk::PhysicalDeviceVulkan11Features{}
                                                           .setShaderDrawParameters(vk::True),
                                                         vk::PhysicalDeviceVulkan13Features{}
                                                           .setDynamicRendering(vk::True),
                                                         vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT{}
                                                           .setExtendedDynamicState(vk::True)};

}