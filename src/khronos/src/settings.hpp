#pragma once

#include <tuple/drop.hpp>
#include <tuple/inner_product.hpp>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_to_string.hpp>

#include <concepts>
#include <functional>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

namespace khronos
{
static constexpr auto min_api_version = vk::ApiVersion14;

static auto const required_device_features = vk::StructureChain{
  vk::PhysicalDeviceFeatures2{},                       //
  vk::PhysicalDeviceVulkan11Features{}                 //
    .setShaderDrawParameters(vk::True),                //
  vk::PhysicalDeviceVulkan13Features{}                 //
    .setDynamicRendering(vk::True)                     //
    .setSynchronization2(vk::True),                    //
  vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT{}  //
    .setExtendedDynamicState(vk::True),                //
};

static auto const required_surface_capabilities = vk::StructureChain{
  vk::SurfaceCapabilities2KHR{},            //
  vk::SurfaceCapabilitiesPresentId2KHR{}    //
    .setPresentId2Supported(vk::True),      //
  vk::SurfaceCapabilitiesPresentWait2KHR{}  //
    .setPresentWait2Supported(vk::True),    //
};

[[nodiscard]]
std::vector<char const *> find_unavailable_extension(std::span<vk::ExtensionProperties const> const properties,
                                                     std::span<char const * const> const            extensions) noexcept;

[[nodiscard]]
std::vector<char const *> find_unavailable_layer(std::span<vk::LayerProperties const> const properties,
                                                 std::span<char const * const> const        layers) noexcept;

[[nodiscard]]
std::vector<char const *> get_required_debug_instance_extensions(std::span<vk::ExtensionProperties const> const properties);

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
bool is_available(vk::PhysicalDeviceFeatures2 const & supported, vk::PhysicalDeviceFeatures2 const & requested) noexcept;

[[nodiscard]]
bool is_available(vk::SurfaceCapabilities2KHR const & supported, vk::SurfaceCapabilities2KHR const & requested) noexcept;

template <class Structure>
[[nodiscard]]
bool is_available(Structure const & supported, Structure const & requested) noexcept
{
  return tuple::inner_product(
    tuple::drop<2>(supported.reflect()),
    tuple::drop<2>(requested.reflect()),
    true,
    std::logical_and<>{},
    [](auto const & supported, auto const & requested)
    {
      if constexpr(std::same_as<decltype(supported), vk::Bool32> and std::same_as<decltype(requested), vk::Bool32>)
        return supported >= requested;
      return true;
    });
}

template <class... Structures>
[[nodiscard]]
std::vector<std::string> find_unavailable_structure_names(vk::StructureChain<Structures...> const & supported,
                                                          vk::StructureChain<Structures...> const & requested) noexcept
{
  return tuple::inner_product(
    static_cast<std::tuple<Structures...> const &>(supported),
    static_cast<std::tuple<Structures...> const &>(requested),
    std::vector<std::string>{},
    [](auto && unavailable_structure_names, auto && unavailable_structure_name)
    {
      if(not unavailable_structure_name.empty())
        unavailable_structure_names.emplace_back(std::move(unavailable_structure_name));
      return std::move(unavailable_structure_names);
    },
    [](auto const & supported, auto const & requested)
    { return is_available(supported, requested) ? std::string{} : vk::to_string(requested.sType); });
}

[[nodiscard]]
inline auto get_features_2(vk::raii::PhysicalDevice const & physical_device) noexcept
{
  return [&]<class... Features>(vk::StructureChain<vk::PhysicalDeviceFeatures2, Features...>)
  {
    return physical_device.getFeatures2<vk::PhysicalDeviceFeatures2, Features...>();
  }(required_device_features);
}

template <class... Args>
[[nodiscard]]
inline auto get_surface_capabilities_2(vk::raii::PhysicalDevice const & physical_device, Args &&... args) noexcept
{
  return [&]<class... Capabilities>(vk::StructureChain<vk::SurfaceCapabilities2KHR, Capabilities...>)
  {
    return physical_device.getSurfaceCapabilities2KHR<vk::SurfaceCapabilities2KHR, Capabilities...>(
      std::forward<Args>(args)...);
  }(required_surface_capabilities);
}
}