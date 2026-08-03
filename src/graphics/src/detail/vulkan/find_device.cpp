#include "find_device.hpp"

#include "../make_shared_with_deleter_data.hpp"

#include <logging/logging.hpp>
#include <logging/serialize.hpp>

#include <algorithm>
#include <memory>
#include <ranges>
#include <unordered_map>

graphics::detail::vulkan::find_device_return_type graphics::detail::vulkan::find_device(
  std::shared_ptr<vk::raii::Instance const> const &   instance,
  std::shared_ptr<vk::raii::SurfaceKHR const> const & surface,
  std::set<std::string>                               device_extensions)
{
  using namespace logging::serialize;

  auto const make_physical_device = [=](vk::raii::PhysicalDevice const & physical_device)
  {
    return make_shared_with_deleter_data<vk::raii::PhysicalDevice const>(instance, physical_device);
  };

  for(auto && physical_device : instance->enumeratePhysicalDevices() | std::views::transform(make_physical_device))
  {
    auto const physical_device_properties = physical_device->getProperties();

    if(physical_device_properties.apiVersion < vk::ApiVersion14)
    {
      logging::warning() << physical_device_properties.deviceName
                         << " is not suitable because does not support vulkan api version 1.4";
      continue;
    }

    auto const is_extension_available =
      [properties = physical_device->enumerateDeviceExtensionProperties()](std::string_view const extension)
    {
      return std::ranges::any_of(properties, [&](auto const & property) { return property.extensionName == extension; });
    };

    auto unavailable_device_extensions = device_extensions | std::views::filter(std::not_fn(is_extension_available));

    if(not unavailable_device_extensions.empty())
    {
      logging::warning() << physical_device_properties.deviceName
                         << " is not suitable because it is missing device extensions: "
                         << logging::to_string(unavailable_device_extensions);
      continue;
    }

    if(is_extension_available(vk::KHRPortabilitySubsetExtensionName))
      device_extensions.emplace(vk::KHRPortabilitySubsetExtensionName);

    if(is_extension_available(vk::KHRSwapchainExtensionName))
      device_extensions.emplace(vk::KHRSwapchainExtensionName);

    auto const required_device_extensions =
      std::vector{std::from_range, device_extensions | std::views::transform([](auto & str) { return str.c_str(); })};

    logging::verbose() << physical_device_properties.deviceName
                       << " has all requires device extensions: " << required_device_extensions;

    auto const [                                        //
      available_physical_device_features,               //
      available_physical_device_features_11,            //
      available_physical_device_features_12,            //
      available_physical_device_features_13,            //
      available_physical_device_features_14,            //
      available_physical_device_features_dynamic_state  //
    ] = physical_device->template getFeatures2<vk::PhysicalDeviceFeatures2,
                                               vk::PhysicalDeviceVulkan11Features,
                                               vk::PhysicalDeviceVulkan12Features,
                                               vk::PhysicalDeviceVulkan13Features,
                                               vk::PhysicalDeviceVulkan14Features,
                                               vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

    if(not available_physical_device_features_11.shaderDrawParameters)
    {
      logging::warning() << physical_device_properties.deviceName
                         << " is not suitable because it is missing a feature: shaderDrawParameters";
      continue;
    }

    if(not available_physical_device_features_13.dynamicRendering)
    {
      logging::warning() << physical_device_properties.deviceName
                         << " is not suitable because it is missing a feature: dynamicRendering";
      continue;
    }

    if(not available_physical_device_features_dynamic_state.extendedDynamicState)
    {
      logging::warning() << physical_device_properties.deviceName
                         << " is not suitable because it is missing a feature: extendedDynamicState";
      continue;
    }

    logging::verbose() << physical_device_properties.deviceName << " has all requires device features";

    auto const properties = physical_device->getQueueFamilyProperties();

    std::uint32_t graphics_queue_index = 0;
    for(; graphics_queue_index < static_cast<std::uint32_t>(properties.size()); graphics_queue_index++)
      if(static_cast<bool>(properties[graphics_queue_index].queueFlags & vk::QueueFlagBits::eGraphics))
        break;

    if(graphics_queue_index == properties.size())
    {
      logging::warning() << physical_device_properties.deviceName
                         << " is not suitable because it is missing the graphics queue family";
      continue;
    }

    logging::verbose() << physical_device_properties.deviceName << " has graphics queue family at index "
                       << graphics_queue_index;

    std::uint32_t present_queue_index = 0;
    for(; present_queue_index < static_cast<std::uint32_t>(properties.size()); present_queue_index++)
      if(physical_device->getSurfaceSupportKHR(present_queue_index, *surface) == vk::True)
        break;

    if(present_queue_index == properties.size())
    {
      logging::warning() << physical_device_properties.deviceName
                         << " is not suitable because it is missing the present queue family";
      continue;
    }

    logging::verbose() << physical_device_properties.deviceName << " has present queue family at index " << present_queue_index;

    constexpr auto queue_priority = std::array{0.5f};

    auto device_queue_create_infos = std::array{
      vk::DeviceQueueCreateInfo{}                   //
        .setQueueFamilyIndex(graphics_queue_index)  //
        .setQueueCount(1)                           //
        .setQueuePriorities(queue_priority),        //
      vk::DeviceQueueCreateInfo{}                   //
        .setQueueFamilyIndex(present_queue_index)   //
        .setQueueCount(1)                           //
        .setQueuePriorities(queue_priority)         //
    };

    auto const queue_family_index_compare = [](vk::DeviceQueueCreateInfo const & lhs, vk::DeviceQueueCreateInfo const & rhs)
    {
      return lhs.queueFamilyIndex == rhs.queueFamilyIndex;
    };

    auto const unique_device_queue_create_infos =
      std::vector{std::from_range, std::ranges::unique(device_queue_create_infos, queue_family_index_compare)};

    auto const required_physical_device_features = vk::StructureChain{
      vk::PhysicalDeviceFeatures2{},                       //
      vk::PhysicalDeviceVulkan11Features{}                 //
        .setShaderDrawParameters(true),                    //
      vk::PhysicalDeviceVulkan12Features{},                //
      vk::PhysicalDeviceVulkan13Features{}                 //
        .setDynamicRendering(true),                        //
      vk::PhysicalDeviceVulkan14Features{},                //
      vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT{}  //
        .setExtendedDynamicState(true)                     //
    };

    auto const device_create_info = vk::DeviceCreateInfo{}                                    //
                                      .setPNext(&required_physical_device_features.get())     //
                                      .setQueueCreateInfos(unique_device_queue_create_infos)  //
                                      .setPEnabledExtensionNames(required_device_extensions);

    auto const device = make_shared_with_deleter_data<vk::raii::Device const>(  //
      physical_device,                                                          //
      *physical_device,                                                         //
      device_create_info);

    auto const create_queue = [&](vk::DeviceQueueCreateInfo const & device_queue_create_info)
    {
      auto const queue_create_info = vk::DeviceQueueInfo2{}                                             //
                                       .setQueueFamilyIndex(device_queue_create_info.queueFamilyIndex)  //
                                       .setQueueIndex(0);

      return std::make_pair(                                   //
        queue_create_info.queueFamilyIndex,                    //
        make_shared_with_deleter_data<vk::raii::Queue const>(  //
          device,                                              //
          *device,                                             //
          queue_create_info));
    };

    auto const queues =
      std::unordered_map{std::from_range, unique_device_queue_create_infos | std::views::transform(create_queue)};

    logging::info() << physical_device_properties.deviceName << " is suitable";

    return {
      .physical_device = physical_device,
      .device          = device,
      .graphics_queue  = queues.find(graphics_queue_index)->second,
      .present_queue   = queues.find(present_queue_index)->second,
    };
  };

  throw std::runtime_error("cannot find suitable gpu");
}