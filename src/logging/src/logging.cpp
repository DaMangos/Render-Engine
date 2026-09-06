#include <logging/logging.hpp>
#include <logging/prefixed_syncbuf.hpp>

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
#else
std::ostream * logging::default_verbose_out = &std::clog;
#endif
std::ostream * logging::default_info_out    = &std::clog;
std::ostream * logging::default_warning_out = &std::cerr;
std::ostream * logging::default_error_out   = &std::cerr;

#ifdef NDEBUG
std::wostream * logging::default_wverbose_out = nullptr;
#else
std::wostream * logging::default_wverbose_out = &std::wclog;
#endif
std::wostream * logging::default_winfo_out    = &std::wclog;
std::wostream * logging::default_wwarning_out = &std::wcerr;
std::wostream * logging::default_werror_out   = &std::wcerr;

logging::prefixed_syncstream logging::verbose(std::ostream * const out)
{
  if(not out)
    return prefixed_syncstream(nullptr);

  return dynamic_cast<std::filebuf const *>(out->rdbuf())
         ? prefixed_syncstream(std::make_unique<prefixed_syncbuf<verbose_prefix>>(*out, verbose_prefix{}))
         : prefixed_syncstream(std::make_unique<prefixed_syncbuf<verbose_colour_prefix>>(*out, verbose_colour_prefix{}));
}

logging::prefixed_syncstream logging::info(std::ostream * const out)
{
  if(not out)
    return prefixed_syncstream(nullptr);

  return dynamic_cast<std::filebuf const *>(out->rdbuf())
         ? prefixed_syncstream(std::make_unique<prefixed_syncbuf<info_prefix>>(*out, info_prefix{}))
         : prefixed_syncstream(std::make_unique<prefixed_syncbuf<info_colour_prefix>>(*out, info_colour_prefix{}));
}

logging::prefixed_syncstream logging::warning(std::ostream * const out)
{
  if(not out)
    return prefixed_syncstream(nullptr);

  return dynamic_cast<std::filebuf const *>(out->rdbuf())
         ? prefixed_syncstream(std::make_unique<prefixed_syncbuf<warn_prefix>>(*out, warn_prefix{}))
         : prefixed_syncstream(std::make_unique<prefixed_syncbuf<warn_colour_prefix>>(*out, warn_colour_prefix{}));
}

logging::prefixed_syncstream logging::error(std::ostream * const out)
{
  if(not out)
    return prefixed_syncstream(nullptr);

  return dynamic_cast<std::filebuf const *>(out->rdbuf())
         ? prefixed_syncstream(std::make_unique<prefixed_syncbuf<error_prefix>>(*out, error_prefix{}))
         : prefixed_syncstream(std::make_unique<prefixed_syncbuf<error_colour_prefix>>(*out, error_colour_prefix{}));
}

logging::wprefixed_syncstream logging::wverbose(std::wostream * const out)
{
  if(not out)
    return wprefixed_syncstream(nullptr);

  return dynamic_cast<std::wfilebuf const *>(out->rdbuf())
         ? wprefixed_syncstream(std::make_unique<wprefixed_syncbuf<wverbose_prefix>>(*out, wverbose_prefix{}))
         : wprefixed_syncstream(std::make_unique<wprefixed_syncbuf<wverbose_colour_prefix>>(*out, wverbose_colour_prefix{}));
}

logging::wprefixed_syncstream logging::winfo(std::wostream * const out)
{
  if(not out)
    return wprefixed_syncstream(nullptr);

  return dynamic_cast<std::wfilebuf const *>(out->rdbuf())
         ? wprefixed_syncstream(std::make_unique<wprefixed_syncbuf<winfo_prefix>>(*out, winfo_prefix{}))
         : wprefixed_syncstream(std::make_unique<wprefixed_syncbuf<winfo_colour_prefix>>(*out, winfo_colour_prefix{}));
}

logging::wprefixed_syncstream logging::wwarning(std::wostream * const out)
{
  if(not out)
    return wprefixed_syncstream(nullptr);

  return dynamic_cast<std::wfilebuf const *>(out->rdbuf())
         ? wprefixed_syncstream(std::make_unique<wprefixed_syncbuf<wwarning_prefix>>(*out, wwarning_prefix{}))
         : wprefixed_syncstream(std::make_unique<wprefixed_syncbuf<wwarning_colour_prefix>>(*out, wwarning_colour_prefix{}));
}

logging::wprefixed_syncstream logging::werror(std::wostream * const out)
{
  if(not out)
    return wprefixed_syncstream(nullptr);

  return dynamic_cast<std::wfilebuf const *>(out->rdbuf())
         ? wprefixed_syncstream(std::make_unique<wprefixed_syncbuf<werror_prefix>>(*out, werror_prefix{}))
         : wprefixed_syncstream(std::make_unique<wprefixed_syncbuf<werror_colour_prefix>>(*out, werror_colour_prefix{}));
}
