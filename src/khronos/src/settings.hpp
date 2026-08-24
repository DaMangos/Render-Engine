#pragma once

#include <tuple/drop.hpp>
#include <tuple/inner_product.hpp>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_to_string.hpp>

#include <functional>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

namespace khronos
{
static auto const required_device_extensions = vk::StructureChain{
  vk::PhysicalDeviceFeatures2{},                       //
  vk::PhysicalDeviceVulkan11Features{}                 //
    .setShaderDrawParameters(vk::True),                //
  vk::PhysicalDeviceVulkan13Features{}                 //
    .setDynamicRendering(vk::True)                     //
    .setSynchronization2(vk::True),                    //
  vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT{}  //
    .setExtendedDynamicState(vk::True),                //
};

[[nodiscard]]
std::vector<char const *> find_unavailable_extension(std::span<vk::ExtensionProperties const> const properties,
                                                     std::span<char const * const> const extensions) noexcept;

[[nodiscard]]
std::vector<char const *> find_unavailable_layer(std::span<vk::LayerProperties const> const properties,
                                                 std::span<char const * const> const        layers) noexcept;

[[nodiscard]]
std::vector<char const *> get_required_debug_instance_extensions(
  std::span<vk::ExtensionProperties const> const properties);

[[nodiscard]]
std::vector<char const *> get_required_instance_extensions(std::span<vk::ExtensionProperties const> const properties);

[[nodiscard]]
std::vector<char const *> get_required_debug_instance_layers();

[[nodiscard]]
std::vector<char const *> get_required_device_extensions(std::span<vk::ExtensionProperties const> const properties);

[[nodiscard]]
bool is_extension_available(std::span<vk::ExtensionProperties const> const properties,
                            std::string_view const                         extension) noexcept;

[[nodiscard]]
bool is_layer_available(std::span<vk::LayerProperties const> const properties, std::string_view const layer) noexcept;

[[nodiscard]]
bool is_feature_available(vk::PhysicalDeviceFeatures2 const & supported,
                          vk::PhysicalDeviceFeatures2 const & requested) noexcept;

template <class Feature>
requires(static_cast<bool>(vk::StructExtends<Feature, vk::PhysicalDeviceFeatures2>::value))
[[nodiscard]]
bool is_feature_available(Feature const & supported, Feature const & requested) noexcept
{
  return tuple::inner_product(tuple::drop<2>(supported.reflect()),
                              tuple::drop<2>(requested.reflect()),
                              true,
                              std::logical_and<>{},
                              std::greater_equal<>{});
}

template <class... Feature>
[[nodiscard]]
std::vector<std::string> find_unavailable_feature_names(
  vk::StructureChain<vk::PhysicalDeviceFeatures2, Feature...> const & supported,
  vk::StructureChain<vk::PhysicalDeviceFeatures2, Feature...> const & requested) noexcept
{
  return tuple::inner_product(
    static_cast<std::tuple<vk::PhysicalDeviceFeatures2, Feature...> const &>(supported),
    static_cast<std::tuple<vk::PhysicalDeviceFeatures2, Feature...> const &>(requested),
    std::vector<std::string>{},
    [](auto && unavailable_feature_names, auto && unavailable_feature_name)
    {
      if(not unavailable_feature_name.empty())
        unavailable_feature_names.emplace_back(std::move(unavailable_feature_name));
      return std::move(unavailable_feature_names);
    },
    [](auto const & supported, auto const & requested)
    { return is_feature_available(supported, requested) ? std::string{} : vk::to_string(requested.sType); });
}

[[nodiscard]]
inline auto get_features_2(vk::raii::PhysicalDevice const & physical_device) noexcept
{
  return [&]<class... Features>(vk::StructureChain<vk::PhysicalDeviceFeatures2, Features...>)
  {
    return physical_device.getFeatures2<vk::PhysicalDeviceFeatures2, Features...>();
  }(required_device_extensions);
}
}