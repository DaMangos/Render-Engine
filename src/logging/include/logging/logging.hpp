#pragma once

#include <logging/prefixedostream.hpp>

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
prefixedostream verbose(std::ostream * const out = default_verbose_out);

[[nodiscard]]
prefixedostream info(std::ostream * const out = default_info_out);

[[nodiscard]]
prefixedostream warning(std::ostream * const out = default_warning_out);

[[nodiscard]]
prefixedostream error(std::ostream * const out = default_error_out);

[[nodiscard]]
wprefixedostream wverbose(std::wostream * const out = default_wverbose_out);

[[nodiscard]]
wprefixedostream winfo(std::wostream * const out = default_winfo_out);

[[nodiscard]]
wprefixedostream wwarning(std::wostream * const out = default_wwarning_out);

[[nodiscard]]
wprefixedostream werror(std::wostream * const out = default_werror_out);
}