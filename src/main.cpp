#include <glfw/library.hpp>
#include <khronos/graphical_device.hpp>
#include <khronos/library.hpp>
#include <khronos/present_window.hpp>
#include <khronos/render_window.hpp>
#include <logging/logging.hpp>
#include <logging/serialize.hpp>

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <span>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <unordered_map>

namespace
{
static constexpr char const * const help_msg
  = "\n\n"
    "--vk-verbose             - Outputs the vulkan verbose logs to the stdlog.\n\n"
    "--vk-verbose=<filename>  - Outputs the vulkan verbose logs to file <filename>.\n\n"
    "--vk-info                - Outputs the vulkan info logs to the stdlog.\n\n"
    "--vk-info=<filename>     - Outputs the vulkan info logs to file <filename>.\n\n"
    "--vk-warning             - Outputs the vulkan warning logs to the stderr.\n\n"
    "--vk-warning=<filename>  - Outputs the vulkan warning logs to file <filename>.\n\n"
    "--vk-error               - Outputs the vulkan error logs to the stderr.\n\n"
    "--vk-error=<filename>    - Outputs the vulkan error logs to file <filename>.\n\n"
    "--verbose                - Outputs the verbose logs to stdlog.\n\n"
    "--verbose=<filename>     - Outputs the verbose logs to file <filename>.\n\n"
    "--no-verbose             - Suppresses the verbose logs.\n\n"
    "--info=<filename>        - Outputs the info logs to stdlog.\n\n"
    "--info=<filename>        - Outputs the info logs to file <filename>.\n\n"
    "--no-info                - Suppresses the info logs.\n\n"
    "--warning                - Outputs the warning logs to stderr.\n\n"
    "--warning=<filename>     - Outputs the warning logs to file <filename>.\n\n"
    "--no-warning             - Suppresses the warning logs.\n\n"
    "--error                  - Outputs the error logs to stderr.\n\n"
    "--error=<filename>       - Outputs the error logs to file <filename>.\n\n"
    "--no-error               - Suppresses the error logs.\n\n";
}

std::ofstream & create_file(std::filesystem::path const & path)
{
  static std::map<std::filesystem::path, std::ofstream> files;

  std::filesystem::create_directories(path.parent_path());

  auto [file, _] = files.try_emplace(path, path, std::ios::out);

  auto & [name, stream] = *file;

  return stream;
}

[[nodiscard]]
int main(int const argc, char const * const * const args) noexcept
{
  using namespace std::literals;

  try
  {
    std::unordered_map<std::string_view, std::ostream *> arg_files;

    for(std::string_view const arg : std::span(std::ranges::next(args), std::ranges::next(args, argc)))
    {
      if(arg == "--help" or arg == "-h")
      {
        std::cout << help_msg << std::endl;

        return EXIT_SUCCESS;
      }
      else if(arg == "--vk-verbose")
      {
        auto const [_, inserted] = arg_files.try_emplace("vk-verbose", &std::clog);

        if(not inserted)
          throw std::invalid_argument("duplicate: --vk-verbose");
      }
      else if(arg.starts_with("--vk-verbose="))
      {
        auto const [_, inserted]
          = arg_files.try_emplace("vk-verbose", &create_file(arg.substr(arg.find_first_of('=') + 1)));

        if(not inserted)
          throw std::invalid_argument("duplicate: --vk-verbose");
      }
      else if(arg == "--vk-info")
      {
        auto const [_, inserted] = arg_files.try_emplace("vk-info", &std::clog);

        if(not inserted)
          throw std::invalid_argument("duplicate: --vk-info");
      }
      else if(arg.starts_with("--vk-info="))
      {
        auto const [_, inserted]
          = arg_files.try_emplace("vk-info", &create_file(arg.substr(arg.find_first_of('=') + 1)));

        if(not inserted)
          throw std::invalid_argument("duplicate: --vk-info");
      }
      else if(arg == "--vk-warning")
      {
        auto const [_, inserted] = arg_files.try_emplace("vk-warning", &std::cerr);

        if(not inserted)
          throw std::invalid_argument("duplicate: --vk-warning");
      }
      else if(arg.starts_with("--vk-warning="))
      {
        auto const [_, inserted]
          = arg_files.try_emplace("vk-warning", &create_file(arg.substr(arg.find_first_of('=') + 1)));

        if(not inserted)
          throw std::invalid_argument("duplicate: --vk-warning");
      }
      else if(arg == "--vk-error")
      {
        auto const [_, inserted] = arg_files.try_emplace("vk-error", &std::cerr);
        if(not inserted)
          throw std::invalid_argument("duplicate: --vk-error");
      }
      else if(arg.starts_with("--vk-error="))
      {
        auto const [_, inserted]
          = arg_files.try_emplace("vk-error", &create_file(arg.substr(arg.find_first_of('=') + 1)));

        if(not inserted)
          throw std::invalid_argument("duplicate: --vk-error");
      }
      else if(arg == "--verbose")
      {
        auto const [_, inserted] = arg_files.try_emplace("verbose", &std::clog);

        if(not inserted)
          throw std::invalid_argument("duplicate: --verbose");
      }
      else if(arg.starts_with("--verbose="))
      {
        auto const [_, inserted]
          = arg_files.try_emplace("verbose", &create_file(arg.substr(arg.find_first_of('=') + 1)));

        if(not inserted)
          throw std::invalid_argument("duplicate: --verbose");
      }
      else if(arg == "--no-verbose")
      {
        auto const [_, inserted] = arg_files.try_emplace("verbose", nullptr);

        if(not inserted)
          throw std::invalid_argument("duplicate: --verbose");
      }
      else if(arg == "--info")
      {
        auto const [_, inserted] = arg_files.try_emplace("info", &std::clog);

        if(not inserted)
          throw std::invalid_argument("duplicate: --info");
      }
      else if(arg.starts_with("--info="))
      {
        auto const [_, inserted]
          = arg_files.try_emplace("info", &create_file(arg.substr(arg.find_first_of('=') + 1)));

        if(not inserted)
          throw std::invalid_argument("duplicate: --info");
      }
      else if(arg == "--no-info")
      {
        auto const [_, inserted] = arg_files.try_emplace("info", nullptr);

        if(not inserted)
          throw std::invalid_argument("duplicate: --info");
      }
      else if(arg == "--warning")
      {
        auto const [_, inserted] = arg_files.try_emplace("warning", &std::cerr);

        if(not inserted)
          throw std::invalid_argument("duplicate: --warning");
      }
      else if(arg.starts_with("--warning="))
      {
        auto const [_, inserted]
          = arg_files.try_emplace("warning", &create_file(arg.substr(arg.find_first_of('=') + 1)));

        if(not inserted)
          throw std::invalid_argument("duplicate: --warning");
      }
      else if(arg == "--no-warning")
      {
        auto const [_, inserted] = arg_files.try_emplace("warning", nullptr);

        if(not inserted)
          throw std::invalid_argument("duplicate: --warning");
      }
      else if(arg == "--error")
      {
        auto const [_, inserted] = arg_files.try_emplace("error", &std::cerr);

        if(not inserted)
          throw std::invalid_argument("duplicate: --error");
      }
      else if(arg.starts_with("--error="))
      {
        auto const [_, inserted]
          = arg_files.try_emplace("error", &create_file(arg.substr(arg.find_first_of('=') + 1)));

        if(not inserted)
          throw std::invalid_argument("duplicate: --error");
      }
      else if(arg == "--no-error")
      {
        auto const [_, inserted] = arg_files.try_emplace("error", nullptr);

        if(not inserted)
          throw std::invalid_argument("duplicate: --error");
      }
      else
        throw std::invalid_argument("vk-unknown: "s + arg);
    }

    if(auto file = arg_files.find("verbose"); file != arg_files.end())
      logging::default_verbose_out = file->second;

    if(auto file = arg_files.find("info"); file != arg_files.end())
      logging::default_info_out = file->second;

    if(auto file = arg_files.find("warning"); file != arg_files.end())
      logging::default_warning_out = file->second;

    if(auto file = arg_files.find("error"); file != arg_files.end())
      logging::default_error_out = file->second;

    khronos::library library{arg_files["vk-verbose"],
                             arg_files["vk-info"],
                             arg_files["vk-warning"],
                             arg_files["vk-error"]};

    auto present_window = library.create_present_window({.height = 500, .width = 500}, "demo");

    auto graphical_device = library.find_graphical_device(present_window);

    auto render_window = graphical_device.create_render_window(std::move(present_window));

    while(not render_window.should_close())
    {
      glfw::default_library.poll_events();

      if(not render_window.is_minimized())
        graphical_device.draw_next_frame(render_window);
    }
  }

  catch(std::system_error const & error)
  {
    logging::error() << error.what();

    return error.code().value();
  }

  catch(std::runtime_error const & error)
  {
    logging::error() << error.what();

    return EXIT_FAILURE;
  }

  catch(std::invalid_argument const & error)
  {
    logging::error() << error.what();

    std::cout << std::endl << help_msg << std::endl;

    return EXIT_FAILURE;
  }

  catch(std::exception const & error)
  {
    logging::error() << error.what();

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}