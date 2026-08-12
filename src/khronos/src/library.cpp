#include "detail/make_shared_with_data.hpp"

#include <glfw/library.hpp>
#include <khronos/graphical_device.hpp>
#include <khronos/library.hpp>
#include <khronos/present_window.hpp>
#include <logging/logging.hpp>
#include <logging/serialize.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <vector>

namespace
{
struct user_data_type
{
    std::ostream * vk_verbose_out = nullptr;
    std::ostream * vk_info_out    = nullptr;
    std::ostream * vk_warning_out = nullptr;
    std::ostream * vk_error_out   = nullptr;
};

static VKAPI_ATTR vk::Bool32 VKAPI_CALL user_callback(vk::DebugUtilsMessageSeverityFlagBitsEXT       severity,
                                                      vk::DebugUtilsMessageTypeFlagsEXT              type,
                                                      vk::DebugUtilsMessengerCallbackDataEXT const * callback_data,
                                                      void *                                         user_data)
{
  if(not user_data)
  {
    logging::error() << "user_data is null";

    return vk::False;
  }

  auto & settings = *static_cast<user_data_type *>(user_data);

  switch(severity)
  {
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose :
    {
      logging::verbose(settings.vk_verbose_out) << vk::to_string(type) << ": " << callback_data->pMessage;

      return vk::False;
    }
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo :
    {
      logging::info(settings.vk_info_out) << vk::to_string(type) << ": " << callback_data->pMessage;

      return vk::False;
    }
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning :
    {
      logging::warning(settings.vk_warning_out) << vk::to_string(type) << ": " << callback_data->pMessage;

      return vk::False;
    }
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError :
    {
      logging::error(settings.vk_error_out) << vk::to_string(type) << ": " << callback_data->pMessage;

      return vk::False;
    }
  }

  logging::error() << "missing severity: " << vk::to_string(type);

  return vk::False;
}

template <typename... Features>
[[nodiscard]]
std::vector<std::size_t> find_unavailable_feature_indices(
  vk::StructureChain<vk::PhysicalDeviceFeatures2, Features...> const & requested_features,
  vk::StructureChain<vk::PhysicalDeviceFeatures2, Features...> const & supported_features)
{
  constexpr auto flatten = [](vk::PhysicalDeviceFeatures2 const & feature, Features const &... features)
  {
    constexpr auto remove_headers = [](auto const & structure)
    {
      return [&]<std::size_t... I>(std::index_sequence<I...>)
      {
        return std::forward_as_tuple(std::get<I + 2>(structure)...);
      }(std::make_index_sequence<std::tuple_size_v<std::decay_t<decltype(structure)>> - 2>());
    };

    return std::tuple_cat(feature.features.reflect(), remove_headers(features.reflect())...);
  };

  constexpr auto to_bitset = [](auto const & flags)
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>)
    {
      std::bitset<std::tuple_size_v<std::decay_t<decltype(flags)>>> bits;

      (bits.set(I, std::get<I>(flags) == vk::True), ...);

      return bits;
    }(std::make_index_sequence<std::tuple_size_v<std::decay_t<decltype(flags)>>>());
  };

  auto const requested_bits   = to_bitset(std::apply(flatten, requested_features));
  auto const supported_bits   = to_bitset(std::apply(flatten, supported_features));
  auto const unavailable_bits = requested_bits & ~supported_bits;

  std::vector<std::size_t> unavailable_features_indices;

  unavailable_features_indices.reserve(unavailable_bits.count());

  for(std::size_t unavailable_index = 0, requested_index = 0; unavailable_index < unavailable_bits.size();
      ++unavailable_index)
  {
    if(unavailable_bits[unavailable_index])
      unavailable_features_indices.emplace_back(requested_index);

    if(requested_bits[unavailable_index])
      requested_index++;
  }

  return unavailable_features_indices;
}
}

khronos::library::library(std::ostream * const vk_verbose_out,
                          std::ostream * const vk_info_out,
                          std::ostream * const vk_warning_out,
                          std::ostream * const vk_error_out)
{
  context = std::make_shared<vk::raii::Context const>();

  auto const is_extension_available =
    [properties = context->enumerateInstanceExtensionProperties()](std::string_view const extension)
  {
    return std::ranges::any_of(properties, [&](auto const & property) { return property.extensionName == extension; });
  };

  auto const is_layer_available = [properties = context
                                                  ->enumerateInstanceLayerProperties()](std::string_view const layer)
  {
    return std::ranges::any_of(properties, [&](auto const & property) { return property.layerName == layer; });
  };

  constexpr auto application_info = vk::ApplicationInfo{}
                                      .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
                                      .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                                      .setApiVersion(vk::ApiVersion14);

  std::vector<char const *> required_instance_extensions{std::from_range,
                                                         glfw::default_library.get_required_instance_extensions()};
  std::vector<char const *> required_instance_layers;

  auto unavailable_instance_extensions = required_instance_extensions
                                       | std::views::filter(std::not_fn(is_extension_available));

  if(not unavailable_instance_extensions.empty())
    throw std::runtime_error("unavailable instance extensions: " + logging::to_string(unavailable_instance_extensions));

  if(is_extension_available(vk::KHRPortabilityEnumerationExtensionName))
  {
    logging::verbose() << "exposing devices with non conformant vulkan implementations";
    required_instance_extensions.emplace_back(vk::KHRPortabilityEnumerationExtensionName);
  }

  if((vk_verbose_out or vk_info_out or vk_warning_out or vk_error_out)
     and is_extension_available(vk::EXTDebugUtilsExtensionName) and is_layer_available("VK_LAYER_KHRONOS_validation"))
  {
    required_instance_layers.emplace_back("VK_LAYER_KHRONOS_validation");

    required_instance_extensions.emplace_back(vk::EXTDebugUtilsExtensionName);

    logging::verbose() << "all required instance extensions are available: " << required_instance_extensions;
    logging::verbose() << "all required instance layers are available: " << required_instance_layers;

    auto const user_data = std::make_shared<user_data_type>(vk_verbose_out, vk_info_out, vk_warning_out, vk_error_out);

    auto const & [instance_create_info, debug_utils_messenger_create_info]
      = vk::StructureChain{vk::InstanceCreateInfo{}
                             .setFlags(is_extension_available(vk::KHRPortabilityEnumerationExtensionName)
                                         ? vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR
                                         : vk::InstanceCreateFlagBits{})
                             .setPApplicationInfo(&application_info)
                             .setPEnabledExtensionNames(required_instance_extensions)
                             .setPEnabledLayerNames(required_instance_layers),
                           vk::DebugUtilsMessengerCreateInfoEXT{}
                             .setMessageSeverity((vk_verbose_out ? vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
                                                                 : vk::DebugUtilsMessageSeverityFlagBitsEXT{})
                                                 | (vk_info_out ? vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
                                                                : vk::DebugUtilsMessageSeverityFlagBitsEXT{})
                                                 | (vk_warning_out ? vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
                                                                   : vk::DebugUtilsMessageSeverityFlagBitsEXT{})
                                                 | (vk_error_out ? vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                                                                 : vk::DebugUtilsMessageSeverityFlagBitsEXT{}))
                             .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                                             | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                                             | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
                             .setPfnUserCallback(&user_callback)
                             .setPUserData(user_data.get())};

    instance = detail::make_shared_with_data<vk::raii::Instance const>(std::make_tuple(context, user_data),
                                                                       *context,
                                                                       instance_create_info);

    debug_utils_messenger = detail::make_shared_with_data<
      vk::raii::DebugUtilsMessengerEXT const>(instance, *instance, debug_utils_messenger_create_info);
  }
  else
  {
    logging::verbose() << "all required instance extensions are available: " << required_instance_extensions;
    logging::verbose() << "all required instance layers are available: " << required_instance_layers;

    auto const instance_create_info = vk::InstanceCreateInfo{}
                                        .setFlags(is_extension_available(vk::KHRPortabilityEnumerationExtensionName)
                                                    ? vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR
                                                    : vk::InstanceCreateFlagBits{})
                                        .setPApplicationInfo(&application_info)
                                        .setPEnabledExtensionNames(required_instance_extensions)
                                        .setPEnabledLayerNames(required_instance_layers);

    instance = detail::make_shared_with_data<vk::raii::Instance const>(context, *context, instance_create_info);
  }
}

khronos::present_window khronos::library::create_present_window(glfw::dimensions<int, 2> size,
                                                                std::string const &      title) const
{
  return {glfw::default_library.create_window(size, title), instance};
}

khronos::present_window khronos::library::create_present_window(glfw::dimensions<int, 2> size,
                                                                std::string const &      title,
                                                                glfw::window const &     share) const
{
  return {glfw::default_library.create_window(size, title, share), instance};
}

khronos::present_window khronos::library::create_present_window(glfw::dimensions<int, 2> size,
                                                                std::string const &      title,
                                                                glfw::monitor const &    monitor) const
{
  return {glfw::default_library.create_window(size, title, monitor), instance};
}

khronos::present_window khronos::library::create_present_window(glfw::dimensions<int, 2> size,
                                                                std::string const &      title,
                                                                glfw::window const &     share,
                                                                glfw::monitor const &    monitor) const
{
  return {glfw::default_library.create_window(size, title, share, monitor), instance};
}

khronos::graphical_device khronos::library::find_graphical_device(present_window const & window) const
{
  return {instance, window.surface};
}