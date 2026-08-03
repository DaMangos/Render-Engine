#include "init_library.hpp"

#include "../make_shared_with_deleter_data.hpp"

#include <logging/logging.hpp>
#include <logging/serialize.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
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
}

graphics::detail::vulkan::init_library_retrun_type graphics::detail::vulkan::init_library(
  std::set<std::string> instance_extensions,
  std::set<std::string> instance_layers,
  std::ostream * const  vk_verbose_out,
  std::ostream * const  vk_info_out,
  std::ostream * const  vk_warning_out,
  std::ostream * const  vk_error_out)
{
  using namespace logging::serialize;

  auto const context = std::make_shared<vk::raii::Context const>();

  auto const is_extension_available =
    [properties = context->enumerateInstanceExtensionProperties()](std::string_view const extension)
  {
    return std::ranges::any_of(properties, [&](auto const & property) { return property.extensionName == extension; });
  };

  auto unavailable_instance_extensions = instance_extensions | std::views::filter(std::not_fn(is_extension_available));

  if(not unavailable_instance_extensions.empty())
    throw std::runtime_error("unavailable instance extensions: " + logging::to_string(unavailable_instance_extensions));

  if(is_extension_available(vk::EXTDebugUtilsExtensionName))
    instance_extensions.emplace(vk::EXTDebugUtilsExtensionName);

  if(is_extension_available(vk::KHRPortabilityEnumerationExtensionName))
    instance_extensions.emplace(vk::KHRPortabilityEnumerationExtensionName);

  auto const required_instance_extensions =
    std::vector{std::from_range, instance_extensions | std::views::transform([](auto & str) { return str.c_str(); })};

  logging::verbose() << "all required instance extensions are available: " << required_instance_extensions;

  auto const is_layer_available = [properties = context->enumerateInstanceLayerProperties()](std::string_view const layer)
  {
    return std::ranges::any_of(properties, [&](auto const & property) { return property.layerName == layer; });
  };

  auto unavailable_instance_layers = instance_layers | std::views::filter(std::not_fn(is_layer_available));

  if(not unavailable_instance_layers.empty())
    throw std::runtime_error("unavailable instance layer: " + logging::to_string(unavailable_instance_layers));

  if((vk_verbose_out or vk_info_out or vk_warning_out or vk_error_out) and is_layer_available("VK_LAYER_KHRONOS_validation"))
    instance_layers.emplace("VK_LAYER_KHRONOS_validation");

  auto const required_instance_layers =
    std::vector{std::from_range, instance_layers | std::views::transform([](auto & str) { return str.c_str(); })};

  logging::verbose() << "all required instance layers are available: " << required_instance_layers;

  constexpr auto application_info = vk::ApplicationInfo{}                               //
                                      .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))  //
                                      .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))       //
                                      .setApiVersion(vk::ApiVersion14);

  if((vk_verbose_out or vk_info_out or vk_warning_out or vk_error_out) and
     is_extension_available(vk::KHRPortabilityEnumerationExtensionName))
  {
    auto const user_data = std::make_shared<user_data_type>(vk_verbose_out, vk_info_out, vk_warning_out, vk_error_out);

    auto const debug_utils_messenger_create_info = vk::DebugUtilsMessengerCreateInfoEXT{}                       //
                                                     .setMessageSeverity(                                       //
                                                       (vk_verbose_out                                          //
                                                          ? vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose  //
                                                          : vk::DebugUtilsMessageSeverityFlagBitsEXT{}) |       //
                                                       (vk_info_out                                             //
                                                          ? vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo     //
                                                          : vk::DebugUtilsMessageSeverityFlagBitsEXT{}) |       //
                                                       (vk_warning_out                                          //
                                                          ? vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning  //
                                                          : vk::DebugUtilsMessageSeverityFlagBitsEXT{}) |       //
                                                       (vk_error_out                                            //
                                                          ? vk::DebugUtilsMessageSeverityFlagBitsEXT::eError    //
                                                          : vk::DebugUtilsMessageSeverityFlagBitsEXT{}))        //
                                                     .setMessageType(                                           //
                                                       vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |         //
                                                       vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |     //
                                                       vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)       //
                                                     .setPfnUserCallback(&user_callback)                        //
                                                     .setPUserData(user_data.get());

    auto const instance_create_info = vk::InstanceCreateInfo{}                                                //
                                        .setPNext(&debug_utils_messenger_create_info)                         //
                                        .setFlags(                                                            //
                                          is_extension_available(vk::KHRPortabilityEnumerationExtensionName)  //
                                            ? vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR            //
                                            : vk::InstanceCreateFlagBits{})                                   //
                                        .setPApplicationInfo(&application_info)                               //
                                        .setPEnabledExtensionNames(required_instance_extensions)              //
                                        .setPEnabledLayerNames(required_instance_layers);

    auto const instance = make_shared_with_deleter_data<vk::raii::Instance const>(  //
      std::make_tuple(context, user_data),
      *context,
      instance_create_info);

    auto const debug_utils_messenger = make_shared_with_deleter_data<vk::raii::DebugUtilsMessengerEXT const>(  //
      instance,
      *instance,
      debug_utils_messenger_create_info);

    return {.context = context, .instance = instance, .debug_utils_messenger = debug_utils_messenger};
  }

  auto const instance_create_info = vk::InstanceCreateInfo{}                                                //
                                      .setFlags(                                                            //
                                        is_extension_available(vk::KHRPortabilityEnumerationExtensionName)  //
                                          ? vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR            //
                                          : vk::InstanceCreateFlagBits{})                                   //
                                      .setPApplicationInfo(&application_info)                               //
                                      .setPEnabledExtensionNames(required_instance_extensions)              //
                                      .setPEnabledLayerNames(required_instance_layers);

  auto const instance = make_shared_with_deleter_data<vk::raii::Instance const>(  //
    context,
    *context,
    instance_create_info);

  return {.context = context, .instance = instance, .debug_utils_messenger = nullptr};
}