#include "settings.hpp"

#include <glfw/library.hpp>

#include <ranges>
#include <vector>

std::vector<char const *> khronos::find_unavailable_extension(std::span<vk::ExtensionProperties const> const properties,
                                                              std::span<char const * const> const extensions) noexcept
{
  auto const is_extension_unavailable = [=](std::string_view const extension)
  {
    return not is_extension_available(properties, extension);
  };

  return extensions | std::views::filter(is_extension_unavailable) | std::ranges::to<std::vector>();
}

std::vector<char const *> khronos::find_unavailable_layer(std::span<vk::LayerProperties const> const properties,
                                                          std::span<char const * const> const        layers) noexcept
{
  auto const is_layer_unavailable = [=](std::string_view const layer)
  {
    return not is_layer_available(properties, layer);
  };

  return layers | std::views::filter(is_layer_unavailable) | std::ranges::to<std::vector>();
}

std::vector<char const *> khronos::get_required_debug_instance_extensions(
  std::span<vk::ExtensionProperties const> const properties)
{
  auto extensions = get_required_instance_extensions(properties);

  extensions.emplace_back(vk::EXTDebugUtilsExtensionName);

  return extensions;
}

std::vector<char const *> khronos::get_required_instance_extensions(
  std::span<vk::ExtensionProperties const> const properties)
{
  auto extensions = glfw::default_library.get_required_instance_extensions() | std::ranges::to<std::vector>();

  if(is_extension_available(properties, vk::KHRPortabilityEnumerationExtensionName))
    extensions.emplace_back(vk::KHRPortabilityEnumerationExtensionName);

  return extensions;
}

std::vector<char const *> khronos::get_required_debug_instance_layers()
{
  return {"VK_LAYER_KHRONOS_validation"};
}

std::vector<char const *> khronos::get_required_device_extensions(
  std::span<vk::ExtensionProperties const> const properties)
{
  auto extensions = std::vector{vk::KHRSwapchainExtensionName, vk::KHRSynchronization2ExtensionName};

  if(is_extension_available(properties, vk::KHRPortabilitySubsetExtensionName))
    extensions.emplace_back(vk::KHRPortabilitySubsetExtensionName);

  return extensions;
}

bool khronos::is_extension_available(std::span<vk::ExtensionProperties const> const properties,
                                     std::string_view const                         extension) noexcept
{
  return std::ranges::any_of(properties, [=](auto const & property) { return property.extensionName == extension; });
}

bool khronos::is_layer_available(std::span<vk::LayerProperties const> const properties,
                                 std::string_view const                     layer) noexcept
{
  return std::ranges::any_of(properties, [=](auto const & property) { return property.layerName == layer; });
}

bool khronos::is_feature_available(vk::PhysicalDeviceFeatures2 const & supported,
                                   vk::PhysicalDeviceFeatures2 const & requested) noexcept
{
  return tuple::inner_product(supported.features.reflect(),
                              requested.features.reflect(),
                              true,
                              std::logical_and<>{},
                              std::greater_equal<>{});
}