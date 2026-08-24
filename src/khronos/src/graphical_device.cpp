#include "graphical_device_impl.hpp"
#include "library_impl.hpp"
#include "make_impl.hpp"
#include "settings.hpp"

#include <glfw/library.hpp>
#include <khronos/graphical_device.hpp>
#include <khronos/graphics_pipeline.hpp>
#include <khronos/library.hpp>
#include <khronos/memory_buffer.hpp>
#include <khronos/present_window.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>
#include <serialize/ranges.hpp>
#include <serialize/tuple.hpp>
#include <tuple/inner_product.hpp>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_to_string.hpp>

#include <cassert>
#include <expected>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <utility>

namespace
{
template <class... Properties>
[[nodiscard]]
static std::string get_device_name(
  vk::StructureChain<vk::PhysicalDeviceProperties2, Properties...> const & device_properties) noexcept
{
  return device_properties.template get<vk::PhysicalDeviceProperties2>().properties.deviceName.data();
}

template <class... Properties>
[[nodiscard]]
static std::string get_driver_name(
  vk::StructureChain<vk::PhysicalDeviceProperties2, Properties...> const & device_properties) noexcept
{
  return device_properties.template get<vk::PhysicalDeviceVulkan12Properties>().driverName.data();
}

template <class... Properties>
[[nodiscard]]
static constexpr std::uint32_t get_api_version(
  vk::StructureChain<vk::PhysicalDeviceProperties2, Properties...> const & device_properties) noexcept
{
  return device_properties.template get<vk::PhysicalDeviceProperties2>().properties.apiVersion;
}

template <class... Properties>
[[nodiscard]]
static std::string get_fancy_device_name(
  vk::StructureChain<vk::PhysicalDeviceProperties2, Properties...> const & device_properties)
{
  return (std::stringstream{} << get_device_name(device_properties) << " : " << get_driver_name(device_properties)
                              << " (" << vk::apiVersionMajor(get_api_version(device_properties))
                              << vk::apiVersionMinor(get_api_version(device_properties))
                              << vk::apiVersionPatch(get_api_version(device_properties)) << ")")
    .str();
}

[[nodiscard]]
static constexpr std::optional<std::uint32_t> find_queue_family_index(
  std::span<vk::QueueFamilyProperties2 const> const queue_family_properties,
  vk::QueueFlags const                              flags) noexcept
{
  for(std::uint32_t queue_family_index = 0; queue_family_index < queue_family_properties.size(); queue_family_index++)
    if((queue_family_properties[queue_family_index].queueFamilyProperties.queueFlags & flags) == flags)
      return queue_family_index;
  return std::nullopt;
}

enum struct physical_device_type
{
  not_suitable,
  non_conformant,
  conformant
};

[[nodiscard]]
static physical_device_type get_physical_device_type(vk::raii::Instance const &       instance,
                                                     vk::raii::PhysicalDevice const & physical_device)

{
  using namespace serialize::tuple;
  using namespace serialize::ranges;

  auto const device_properties = physical_device.getProperties2<vk::PhysicalDeviceProperties2,
                                                                vk::PhysicalDeviceVulkan12Properties>();

  if(get_api_version(device_properties) >= vk::ApiVersion13)
  {
    logging::warning() << get_fancy_device_name(device_properties)
                       << " is not suitable because it only supports vulkan api version "
                       << vk::apiVersionMajor(get_api_version(device_properties)) << '.'
                       << vk::apiVersionMinor(get_api_version(device_properties)) << '.'
                       << vk::apiVersionPatch(get_api_version(device_properties)) << " and we require "
                       << vk::apiVersionMajor(vk::ApiVersion13) << '.' << vk::apiVersionMinor(vk::ApiVersion13) << '.'
                       << vk::apiVersionPatch(vk::ApiVersion13);

    return physical_device_type::not_suitable;
  }

  auto const extension_properties   = physical_device.enumerateDeviceExtensionProperties();
  auto const required_extensions    = khronos::get_required_device_extensions(extension_properties);
  auto const unavailable_extensions = khronos::find_unavailable_extension(extension_properties, required_extensions);

  if(not unavailable_extensions.empty())
  {
    logging::warning() << get_fancy_device_name(device_properties)
                       << " is not suitable because it's missing device extensions:" << unavailable_extensions;

    return physical_device_type::not_suitable;
  }

  auto const device_features = [&]<class... Features>(vk::StructureChain<vk::PhysicalDeviceFeatures2, Features...>)
  {
    return physical_device.getFeatures2<vk::PhysicalDeviceFeatures2, Features...>();
  }(khronos::required_device_extensions);

  auto const unavailable_feature_names = khronos::find_unavailable_feature_names(device_features,
                                                                                 khronos::required_device_extensions);

  if(unavailable_feature_names.empty())
  {
    logging::warning() << get_fancy_device_name(device_properties)
                       << " is not suitable because it's missing required physical device features at indices "
                       << unavailable_feature_names;

    return physical_device_type::not_suitable;
  }

  auto const queue_family_properties = physical_device.getQueueFamilyProperties2();
  auto const queue_family_flags      = vk::QueueFlagBits::eTransfer | vk::QueueFlagBits::eGraphics;
  auto const queue_family_index      = find_queue_family_index(queue_family_properties, queue_family_flags);

  if(not queue_family_index
     or not glfw::default_library.get_physical_device_presentation_support(instance,
                                                                           physical_device,
                                                                           *queue_family_index))
  {
    logging::warning()
      << get_fancy_device_name(device_properties)
      << " is not suitable because it's missing the transfer, graphics, and present queue family on index";

    return physical_device_type::not_suitable;
  }

  if(khronos::is_extension_available(extension_properties, vk::KHRPortabilitySubsetExtensionName))
  {
    logging::info() << get_fancy_device_name(device_properties) << " is suitable but non conformant";

    return physical_device_type::non_conformant;
  }

  logging::info() << get_fancy_device_name(device_properties) << " is suitable and conformant";

  return physical_device_type::conformant;
}

[[nodiscard]]
static vk::raii::PhysicalDevice find_physical_device(vk::raii::Instance const & instance)
{
  std::unordered_map<physical_device_type, vk::raii::PhysicalDevice> physical_devices;

  for(auto const & physical_device : instance.enumeratePhysicalDevices())
    physical_devices.try_emplace(get_physical_device_type(instance, physical_device), physical_device);

  auto const conformant_physical_device = physical_devices.find(physical_device_type::conformant);

  if(conformant_physical_device != physical_devices.end())
    return conformant_physical_device->second;

  auto const non_conformant_physical_device = physical_devices.find(physical_device_type::conformant);

  if(non_conformant_physical_device != physical_devices.end())
    return non_conformant_physical_device->second;

  throw std::runtime_error("there are no suitable physical devices");
}

[[nodiscard]]
static std::unique_ptr<
  khronos::graphical_device_impl,
  void (*)(khronos::graphical_device_impl *)> create_graphical_device_impl(khronos::instance const & instance)
{
  auto physical_device = khronos::physical_device(instance.as_dependencies(), find_physical_device(instance.get()));

  auto const queue_family_properties = physical_device.get().getQueueFamilyProperties2();
  auto const queue_family_flags      = vk::QueueFlagBits::eTransfer | vk::QueueFlagBits::eGraphics;
  auto const queue_family_index      = find_queue_family_index(queue_family_properties, queue_family_flags);
  auto const queue_priority          = 0.5f;

  auto const queue_create_infos = vk::DeviceQueueCreateInfo{}
                                    .setQueueFamilyIndex(*queue_family_index)  //
                                    .setQueueCount(1)                          //
                                    .setQueuePriorities(queue_priority);       //

  auto const extension_properties = physical_device.get().enumerateDeviceExtensionProperties();
  auto const required_extensions  = khronos::get_required_device_extensions(extension_properties);

  auto const device_create_info = vk::StructureChain{
    vk::DeviceCreateInfo{}                              //
      .setQueueCreateInfos(queue_create_infos)          //
      .setPEnabledExtensionNames(required_extensions),  //
    khronos::required_device_extensions.get(),          //
  };

  auto device = khronos::device(physical_device.as_dependencies(), physical_device.get(), device_create_info.get());

  auto const device_queue_info = vk::DeviceQueueInfo2{}
                                   .setQueueFamilyIndex(*queue_family_index)  //
                                   .setQueueIndex(0);                         //

  auto queue = khronos::queue(device.as_dependencies(), device.get(), device_queue_info);

  return khronos::make_impl<khronos::graphical_device_impl>(std::move(physical_device),
                                                            std::move(device),
                                                            std::move(queue));
}
}

khronos::graphical_device::graphical_device(library const & library)
: ptr(create_graphical_device_impl(library.ptr->instance))
{
}