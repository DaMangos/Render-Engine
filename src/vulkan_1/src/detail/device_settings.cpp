#include "device_settings.hpp"

#include <algorithm>
#include <expected>
#include <functional>
#include <string_view>
#include <vector>

std::expected<std::vector<char const *>, std::vector<char const *>> vulkan_1::detail::get_required_device_extensions(
  std::shared_ptr<vk::raii::PhysicalDevice const> const & physical_device)
{
  auto const is_extension_available
    = [properties = physical_device->enumerateDeviceExtensionProperties()](std::string_view const extension)
  {
    return std::ranges::any_of(properties, [&](auto const & property) { return property.extensionName == extension; });
  };

  auto required_device_extensions = std::vector{vk::KHRSwapchainExtensionName};

  auto unavailable_device_extensions = required_device_extensions;

  std::erase_if(unavailable_device_extensions, is_extension_available);

  if(not unavailable_device_extensions.empty())
    return std::unexpected{unavailable_device_extensions};

  if(is_extension_available(vk::KHRPortabilitySubsetExtensionName))
    required_device_extensions.emplace_back(vk::KHRPortabilitySubsetExtensionName);

  return required_device_extensions;
}
