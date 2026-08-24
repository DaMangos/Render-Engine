#pragma once

#include <logging/prefixed_syncstream.hpp>

#include <ostream>

namespace logging
{
extern std::ostream * default_verbose_out;
extern std::ostream * default_info_out;
extern std::ostream * default_warning_out;
extern std::ostream * default_error_out;

extern std::wostream * default_wverbose_out;
extern std::wostream * default_winfo_out;
extern std::wostream * default_wwarning_out;
extern std::wostream * default_werror_out;

[[nodiscard]]
prefixed_syncstream verbose(std::ostream * const out = default_verbose_out);

[[nodiscard]]
prefixed_syncstream info(std::ostream * const out = default_info_out);

[[nodiscard]]
prefixed_syncstream warning(std::ostream * const out = default_warning_out);

[[nodiscard]]
prefixed_syncstream error(std::ostream * const out = default_error_out);

[[nodiscard]]
wprefixed_syncstream wverbose(std::wostream * const out = default_wverbose_out);

[[nodiscard]]
wprefixed_syncstream winfo(std::wostream * const out = default_winfo_out);

[[nodiscard]]
wprefixed_syncstream wwarning(std::wostream * const out = default_wwarning_out);

[[nodiscard]]
wprefixed_syncstream werror(std::wostream * const out = default_werror_out);
}