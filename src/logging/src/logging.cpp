#include <logging/logging.hpp>
#include <logging/prefixedbuf.hpp>

#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

namespace
{
struct verbose_colour_prefix
{
    [[nodiscard]]
    std::string operator()() const
    {
      return std::format("\033[1m[{:%Y-%m-%d %H:%M:%S}] \033[1;35m[ VERBOSE ]\033[0m ", std::chrono::system_clock::now());
    }
};

struct verbose_prefix
{
    [[nodiscard]]
    std::string operator()() const
    {
      return std::format("[{:%Y-%m-%d %H:%M:%S}] [ VERBOSE ] ", std::chrono::system_clock::now());
    }
};

struct wverbose_colour_prefix
{
    [[nodiscard]]
    std::wstring operator()() const
    {
      return std::format(L"\033[1m[{:%Y-%m-%d %H:%M:%S}] \033[1;35m[ VERBOSE ]\033[0m ", std::chrono::system_clock::now());
    }
};

struct wverbose_prefix
{
    [[nodiscard]]
    std::wstring operator()() const
    {
      return std::format(L"[{:%Y-%m-%d %H:%M:%S}] [ VERBOSE ] ", std::chrono::system_clock::now());
    }
};

struct info_colour_prefix
{
    [[nodiscard]]
    std::string operator()() const
    {
      return std::format("\033[1m[{:%Y-%m-%d %H:%M:%S}] \033[1;34m[ INFO ]\033[0m ", std::chrono::system_clock::now());
    }
};

struct info_prefix
{
    [[nodiscard]]
    std::string operator()() const
    {
      return std::format("[{:%Y-%m-%d %H:%M:%S}] [ INFO ] ", std::chrono::system_clock::now());
    }
};

struct winfo_colour_prefix
{
    [[nodiscard]]
    std::wstring operator()() const
    {
      return std::format(L"\033[1m[{:%Y-%m-%d %H:%M:%S}] \033[1;34m[ INFO ]\033[0m ", std::chrono::system_clock::now());
    }
};

struct winfo_prefix
{
    [[nodiscard]]
    std::wstring operator()() const
    {
      return std::format(L"[{:%Y-%m-%d %H:%M:%S}] [ INFO ]", std::chrono::system_clock::now());
    }
};

struct warn_colour_prefix
{
    [[nodiscard]]
    std::string operator()() const
    {
      return std::format("\033[1m[{:%Y-%m-%d %H:%M:%S}] \033[1;33m[ WARNING ]\033[0m ", std::chrono::system_clock::now());
    }
};

struct warn_prefix
{
    [[nodiscard]]
    std::string operator()() const
    {
      return std::format("[{:%Y-%m-%d %H:%M:%S}] [ WARNING ] ", std::chrono::system_clock::now());
    }
};

struct wwarning_colour_prefix
{
    [[nodiscard]]
    std::wstring operator()() const
    {
      return std::format(L"\033[1m[{:%Y-%m-%d %H:%M:%S}] \033[1;33m[ WARNING ]\033[0m ", std::chrono::system_clock::now());
    }
};

struct wwarning_prefix
{
    [[nodiscard]]
    std::wstring operator()() const
    {
      return std::format(L"[{:%Y-%m-%d %H:%M:%S}] [ WARNING ] ", std::chrono::system_clock::now());
    }
};

struct error_colour_prefix
{
    [[nodiscard]]
    std::string operator()() const
    {
      return std::format("\033[1m[{:%Y-%m-%d %H:%M:%S}] \033[1;31m[ ERROR ]\033[0m ", std::chrono::system_clock::now());
    }
};

struct error_prefix
{
    [[nodiscard]]
    std::string operator()() const
    {
      return std::format("[{:%Y-%m-%d %H:%M:%S}] [ ERROR ] ", std::chrono::system_clock::now());
    }
};

struct werror_colour_prefix
{
    [[nodiscard]]
    std::wstring operator()() const
    {
      return std::format(L"\033[1m[{:%Y-%m-%d %H:%M:%S}] \033[1;31m[ ERROR ]\033[0m ", std::chrono::system_clock::now());
    }
};

struct werror_prefix
{
    [[nodiscard]]
    std::wstring operator()() const
    {
      return std::format(L"[{:%Y-%m-%d %H:%M:%S}] [ ERROR ] ", std::chrono::system_clock::now());
    }
};
}

#ifdef NDEBUG
std::ostream * logging::default_verbose_out = nullptr;
std::ostream * logging::default_info_out    = nullptr;
#else
std::ostream * logging::default_verbose_out = &std::clog;
std::ostream * logging::default_info_out    = &std::clog;
#endif
std::ostream * logging::default_warning_out = &std::cerr;
std::ostream * logging::default_error_out   = &std::cerr;

#ifdef NDEBUG
std::wostream * logging::default_wverbose_out = nullptr;
std::wostream * logging::default_winfo_out    = nullptr;
#else
std::wostream * logging::default_wverbose_out = &std::wclog;
std::wostream * logging::default_winfo_out    = &std::wclog;
#endif
std::wostream * logging::default_wwarning_out = &std::wcerr;
std::wostream * logging::default_werror_out   = &std::wcerr;

logging::prefixedostream logging::verbose(std::ostream * const out)
{
  if(not out)
    return prefixedostream(nullptr);

  return dynamic_cast<std::filebuf const *>(out->rdbuf())
         ? prefixedostream(std::make_unique<prefixedbuf<verbose_prefix>>(*out, verbose_prefix{}))
         : prefixedostream(std::make_unique<prefixedbuf<verbose_colour_prefix>>(*out, verbose_colour_prefix{}));
}

logging::prefixedostream logging::info(std::ostream * const out)
{
  if(not out)
    return prefixedostream(nullptr);

  return dynamic_cast<std::filebuf const *>(out->rdbuf())
         ? prefixedostream(std::make_unique<prefixedbuf<info_prefix>>(*out, info_prefix{}))
         : prefixedostream(std::make_unique<prefixedbuf<info_colour_prefix>>(*out, info_colour_prefix{}));
}

logging::prefixedostream logging::warning(std::ostream * const out)
{
  if(not out)
    return prefixedostream(nullptr);

  return dynamic_cast<std::filebuf const *>(out->rdbuf())
         ? prefixedostream(std::make_unique<prefixedbuf<warn_prefix>>(*out, warn_prefix{}))
         : prefixedostream(std::make_unique<prefixedbuf<warn_colour_prefix>>(*out, warn_colour_prefix{}));
}

logging::prefixedostream logging::error(std::ostream * const out)
{
  if(not out)
    return prefixedostream(nullptr);

  return out and dynamic_cast<std::filebuf const *>(out->rdbuf())
         ? prefixedostream(std::make_unique<prefixedbuf<error_prefix>>(*out, error_prefix{}))
         : prefixedostream(std::make_unique<prefixedbuf<error_colour_prefix>>(*out, error_colour_prefix{}));
}

logging::wprefixedostream logging::wverbose(std::wostream * const out)
{
  if(not out)
    return wprefixedostream(nullptr);

  return dynamic_cast<std::wfilebuf const *>(out->rdbuf())
         ? wprefixedostream(std::make_unique<wprefixedbuf<wverbose_prefix>>(*out, wverbose_prefix{}))
         : wprefixedostream(std::make_unique<wprefixedbuf<wverbose_colour_prefix>>(*out, wverbose_colour_prefix{}));
}

logging::wprefixedostream logging::winfo(std::wostream * const out)
{
  if(not out)
    return wprefixedostream(nullptr);

  return dynamic_cast<std::wfilebuf const *>(out->rdbuf())
         ? wprefixedostream(std::make_unique<wprefixedbuf<winfo_prefix>>(*out, winfo_prefix{}))
         : wprefixedostream(std::make_unique<wprefixedbuf<winfo_colour_prefix>>(*out, winfo_colour_prefix{}));
}

logging::wprefixedostream logging::wwarning(std::wostream * const out)
{
  if(not out)
    return wprefixedostream(nullptr);

  return dynamic_cast<std::wfilebuf const *>(out->rdbuf())
         ? wprefixedostream(std::make_unique<wprefixedbuf<wwarning_prefix>>(*out, wwarning_prefix{}))
         : wprefixedostream(std::make_unique<wprefixedbuf<wwarning_colour_prefix>>(*out, wwarning_colour_prefix{}));
}

logging::wprefixedostream logging::werror(std::wostream * const out)
{
  if(not out)
    return wprefixedostream(nullptr);

  return dynamic_cast<std::wfilebuf const *>(out->rdbuf())
         ? wprefixedostream(std::make_unique<wprefixedbuf<werror_prefix>>(*out, werror_prefix{}))
         : wprefixedostream(std::make_unique<wprefixedbuf<werror_colour_prefix>>(*out, werror_colour_prefix{}));
}
