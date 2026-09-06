#include "dependencies.hpp"
#include "impl.hpp"
#include "settings.hpp"

#include <glfw/library.hpp>
#include <khronos/library.hpp>
#include <logging/logging.hpp>
#include <serialize/ranges.hpp>
#include <serialize/tuple.hpp>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_to_string.hpp>

#include <algorithm>
#include <cassert>
#include <optional>
#include <string_view>
#include <vector>

namespace
{
static vk::Bool32 user_callback(vk::DebugUtilsMessageSeverityFlagBitsEXT       severity,
                                vk::DebugUtilsMessageTypeFlagsEXT              type,
                                vk::DebugUtilsMessengerCallbackDataEXT const * callback_data,
                                void *                                         user_data)
{
  if(not user_data)
  {
    logging::error() << "user_data is null";

    return vk::False;
  }

  switch(severity)
  {
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose :
    {
      logging::verbose(static_cast<khronos::user_data *>(user_data)->vk_verbose_out)
        << vk::to_string(type) << ": " << callback_data->pMessage;

      return vk::False;
    }
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo :
    {
      logging::info(static_cast<khronos::user_data *>(user_data)->vk_info_out)
        << vk::to_string(type) << ": " << callback_data->pMessage;

      return vk::False;
    }
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning :
    {
      logging::warning(static_cast<khronos::user_data *>(user_data)->vk_warning_out)
        << vk::to_string(type) << ": " << callback_data->pMessage;

      return vk::False;
    }
    case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError :
    {
      logging::error(static_cast<khronos::user_data *>(user_data)->vk_error_out)
        << vk::to_string(type) << ": " << callback_data->pMessage;

      return vk::False;
    }
  }

  logging::error() << "missing severity: " << vk::to_string(type);

  return vk::False;
}

[[nodiscard]]
static constexpr vk::InstanceCreateFlagBits get_instance_creat_info_flags(
  std::span<char const * const> const extensions) noexcept
{
  return std::ranges::contains(extensions, std::string_view{vk::KHRPortabilityEnumerationExtensionName})
         ? vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR
         : vk::InstanceCreateFlagBits{};
}

[[nodiscard]]
static constexpr vk::DebugUtilsMessageSeverityFlagsEXT get_debug_utils_message_severity(
  std::ostream * const vk_verbose_out,
  std::ostream * const vk_info_out,
  std::ostream * const vk_warning_out,
  std::ostream * const vk_error_out) noexcept
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
}

[[nodiscard]]
static khronos::library_impl create_library_impl(std::ostream * const vk_verbose_out,
                                                 std::ostream * const vk_info_out,
                                                 std::ostream * const vk_warning_out,
                                                 std::ostream * const vk_error_out)
{
  assert(vk_verbose_out or vk_info_out or vk_warning_out or vk_error_out);

  using namespace serialize::tuple;
  using namespace serialize::ranges;

  khronos::context context;

  auto const available_extension = context.get().enumerateInstanceExtensionProperties();

  auto const required_extensions = khronos::get_required_debug_instance_extensions(available_extension);

  auto const unavailable_extensions = khronos::find_unavailable_extension(available_extension, required_extensions);

  if(not unavailable_extensions.empty())
    throw std::runtime_error((std::stringstream{} << "unavailable instance extensions: " << unavailable_extensions).str());

  logging::verbose() << "all required instance extensions are available: " << required_extensions;

  auto const available_layers = context.get().enumerateInstanceLayerProperties();

  auto const required_layers = khronos::get_required_debug_instance_layers();

  auto const unavailable_layers = khronos::find_unavailable_layer(available_layers, required_layers);

  if(not unavailable_layers.empty())
    throw std::runtime_error((std::stringstream{} << "unavailable instance layers: " << unavailable_layers).str());

  logging::verbose() << "all required instance layers are available: " << required_layers;

  auto const application_info = vk::ApplicationInfo{}.setApiVersion(khronos::min_api_version);

  auto user_data = khronos::dependent<khronos::user_data>{
    vk_verbose_out,
    vk_info_out,
    vk_warning_out,
    vk_error_out,
  };

  auto const instance_dependencies = khronos::dependency_union(context.as_dependencies(), user_data.as_dependencies());

  auto const message_severity = get_debug_utils_message_severity(vk_verbose_out, vk_info_out, vk_warning_out, vk_error_out);

  auto const message_type = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                          | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;

  auto const & [instance_create_info, debug_utils_messenger_create_info] = vk::StructureChain{
    vk::InstanceCreateInfo{}
      .setFlags(get_instance_creat_info_flags(required_extensions))
      .setPApplicationInfo(&application_info)
      .setPEnabledExtensionNames(required_extensions)
      .setPEnabledLayerNames(required_layers),
    vk::DebugUtilsMessengerCreateInfoEXT{}
      .setMessageSeverity(message_severity)
      .setMessageType(message_type)
      .setPfnUserCallback(&user_callback)
      .setPUserData(&user_data.get())};

  auto instance = khronos::instance{instance_dependencies, context, instance_create_info};

  auto debug_utils_messenger = khronos::debug_utils_messenger{instance.as_dependencies(),
                                                              instance,
                                                              debug_utils_messenger_create_info};

  return {std::move(context), std::move(instance), std::move(debug_utils_messenger)};
}

[[nodiscard]]
static khronos::library_impl create_library_impl()
{
  using namespace serialize::tuple;
  using namespace serialize::ranges;

  khronos::context context;

  auto const available_extension = context.get().enumerateInstanceExtensionProperties();

  auto const required_extensions = khronos::get_required_instance_extensions(available_extension);

  auto const unavailable_extensions = khronos::find_unavailable_extension(available_extension, required_extensions);

  if(not unavailable_extensions.empty())
    throw std::runtime_error((std::stringstream{} << "unavailable instance extensions: " << unavailable_extensions).str());

  logging::verbose() << "all required instance extensions are available: " << required_extensions;

  auto const application_info = vk::ApplicationInfo{}.setApiVersion(vk::ApiVersion13);

  auto const instance_create_info = vk::InstanceCreateInfo{}
                                      .setFlags(get_instance_creat_info_flags(required_extensions))
                                      .setPApplicationInfo(&application_info)
                                      .setPEnabledExtensionNames(required_extensions);

  auto const user_data = khronos::dependent<khronos::user_data>{};

  auto const instance_dependencies = khronos::dependency_union(context.as_dependencies(), user_data.as_dependencies());

  auto instance = khronos::instance{instance_dependencies, context, instance_create_info};

  return {std::move(context), std::move(instance), std::nullopt};
}
}

khronos::library::library(std::ostream * const vk_verbose_out,
                          std::ostream * const vk_info_out,
                          std::ostream * const vk_warning_out,
                          std::ostream * const vk_error_out)
: self(vk_verbose_out or vk_info_out or vk_warning_out or vk_error_out
         ? std::make_unique<library_impl>(create_library_impl(vk_verbose_out, vk_info_out, vk_warning_out, vk_error_out))
         : std::make_unique<library_impl>(create_library_impl()))
{
}

khronos::library::~library() noexcept = default;
