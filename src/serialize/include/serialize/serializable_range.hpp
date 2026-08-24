#pragma once

/*
synopsis


namespace serialize
{
template <class Range, class CharT, class Traits>
concept serializable_range;
}

*/

#include <concepts>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>

namespace serialize
{
template <class Range, class CharT = char, class Traits = std::char_traits<CharT>>
concept serializable_range = std::ranges::input_range<std::decay_t<Range>>
                         and not(std::convertible_to<std::decay_t<Range>, std::basic_string_view<CharT, Traits>>
                                 or std::convertible_to<std::decay_t<Range>, std::basic_string<CharT, Traits>>);
}
