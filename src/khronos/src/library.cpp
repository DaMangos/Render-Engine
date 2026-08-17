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
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <vector>

namespace
{
namespace detail
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
}

khronos::library::library(std::ostream * const vk_verbose_out,
                          std::ostream * const vk_info_out,
                          std::ostream * const vk_warning_out,
                          std::ostream * const vk_error_out)
{
  using namespace logging::serialize;

  context = std::make_shared<vk::raii::Context const>();

  auto const is_extension_available
    = [properties = context->enumerateInstanceExtensionProperties()](std::string_view const extension)
  {
    return std::ranges::any_of(properties, [&](auto const & property) { return property.extensionName == extension; });
  };

  auto const is_extension_unavailable = std::not_fn(is_extension_available);

  auto extensions = glfw::default_library.get_required_instance_extensions() | std::ranges::to<std::vector>();

  auto unavailable_extensions = extensions | std::views::filter(is_extension_unavailable);

  if(not unavailable_extensions.empty())
    throw std::runtime_error(
      (std::stringstream{} << "unavailable instance extensions: " << unavailable_extensions).str());

  bool const expose_non_conformant_implementations = is_extension_available(vk::KHRPortabilityEnumerationExtensionName);

  auto const flags = expose_non_conformant_implementations ? vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR
                                                           : vk::InstanceCreateFlagBits{};
  if(expose_non_conformant_implementations)
  {
    logging::verbose() << "exposing physical devices with non conformant vulkan implementations";

    extensions.emplace_back(vk::KHRPortabilityEnumerationExtensionName);
  }

  auto const application_info = vk::ApplicationInfo{}.setApiVersion(context->enumerateInstanceVersion());

  if(not vk_verbose_out and not vk_info_out and not vk_warning_out and not vk_error_out)
  {
    logging::verbose() << "all required instance extensions are available: " << extensions;

    auto const instance_create_info = vk::InstanceCreateInfo{}
                                        .setFlags(flags)
                                        .setPApplicationInfo(&application_info)
                                        .setPEnabledExtensionNames(extensions);

    instance = detail::make_shared_with_data<vk::raii::Instance const>(*context, instance_create_info);

    detail::emplace_data(instance, context);

    return;
  }

  auto const is_layer_unavailable
    = [properties = context->enumerateInstanceLayerProperties()](std::string_view const layer)
  {
    return std::ranges::none_of(properties, [&](auto const & property) { return property.layerName == layer; });
  };

  if(is_layer_unavailable("VK_LAYER_KHRONOS_validation") or is_extension_unavailable(vk::EXTDebugUtilsExtensionName))
    throw std::runtime_error("cannot create instance with validation layers");

  extensions.emplace_back(vk::EXTDebugUtilsExtensionName);

  logging::verbose() << "all required instance extensions are available: " << extensions;

  auto const user_data
    = std::make_shared<::detail::user_data_type>(vk_verbose_out, vk_info_out, vk_warning_out, vk_error_out);

  auto const message_severity = [=]()
  {
    auto message_severity = vk::DebugUtilsMessageSeverityFlagsEXT{};

    if(vk_verbose_out)
      message_severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;

    if(vk_info_out)
      message_severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;

    if(vk_warning_out)
      message_severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;

    if(vk_error_out)
      message_severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;

    return message_severity;
  }();

  auto const message_type = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                          | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                          | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;

  constexpr auto layers = "VK_LAYER_KHRONOS_validation";

  auto const & [instance_create_info, debug_utils_messenger_create_info]
    = vk::StructureChain{vk::InstanceCreateInfo{}
                           .setFlags(flags)
                           .setPApplicationInfo(&application_info)
                           .setPEnabledExtensionNames(extensions)
                           .setPEnabledLayerNames(layers),
                         vk::DebugUtilsMessengerCreateInfoEXT{}
                           .setMessageSeverity(message_severity)
                           .setMessageType(message_type)
                           .setPfnUserCallback(&::detail::user_callback)
                           .setPUserData(user_data.get())};

  instance = detail::make_shared_with_data<vk::raii::Instance const>(*context, instance_create_info);

  detail::emplace_data(instance, context);
  detail::emplace_data(instance, user_data);

  debug_utils_messenger
    = detail::make_shared_with_data<vk::raii::DebugUtilsMessengerEXT const>(*instance,
                                                                            debug_utils_messenger_create_info);

  detail::emplace_data(debug_utils_messenger, instance);
}

khronos::present_window khronos::library::create_present_window(glfw::dimensions<int, 2> const & size,
                                                                std::string const &              title) const
{
  return {glfw::default_library.create_window(size, title), instance};
}

khronos::present_window khronos::library::create_present_window(glfw::dimensions<int, 2> const & size,
                                                                std::string const &              title,
                                                                glfw::window const &             share) const
{
  return {glfw::default_library.create_window(size, title, share), instance};
}

khronos::present_window khronos::library::create_present_window(glfw::dimensions<int, 2> const & size,
                                                                std::string const &              title,
                                                                glfw::monitor const &            monitor) const
{
  return {glfw::default_library.create_window(size, title, monitor), instance};
}

khronos::present_window khronos::library::create_present_window(glfw::dimensions<int, 2> const & size,
                                                                std::string const &              title,
                                                                glfw::window const &             share,
                                                                glfw::monitor const &            monitor) const
{
  return {glfw::default_library.create_window(size, title, share, monitor), instance};
}

khronos::graphical_device khronos::library::find_graphical_device(present_window const & window) const
{
  return {context, instance, window.surface};
}