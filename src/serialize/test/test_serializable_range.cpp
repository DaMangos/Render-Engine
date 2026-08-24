#include <serialize/serializable_range.hpp>

#include <array>
#include <deque>
#include <filesystem>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static_assert(serialize::serializable_range<std::vector<char>>);
static_assert(serialize::serializable_range<std::vector<wchar_t>>);
static_assert(serialize::serializable_range<std::deque<char>>);
static_assert(serialize::serializable_range<std::deque<wchar_t>>);
static_assert(serialize::serializable_range<std::array<char, 1>>);
static_assert(serialize::serializable_range<std::array<wchar_t, 1>>);
static_assert(serialize::serializable_range<std::list<char>>);
static_assert(serialize::serializable_range<std::list<wchar_t>>);
static_assert(serialize::serializable_range<std::forward_list<char>>);
static_assert(serialize::serializable_range<std::forward_list<wchar_t>>);
static_assert(serialize::serializable_range<std::set<char>>);
static_assert(serialize::serializable_range<std::set<wchar_t>>);
static_assert(serialize::serializable_range<std::unordered_set<char>>);
static_assert(serialize::serializable_range<std::unordered_set<wchar_t>>);
static_assert(serialize::serializable_range<std::map<char, char>>);
static_assert(serialize::serializable_range<std::map<wchar_t, wchar_t>>);
static_assert(serialize::serializable_range<std::unordered_map<char, char>>);
static_assert(serialize::serializable_range<std::unordered_map<wchar_t, wchar_t>>);
static_assert(not serialize::serializable_range<std::string>);
static_assert(not serialize::serializable_range<std::wstring, wchar_t>);
static_assert(not serialize::serializable_range<std::string_view>);
static_assert(not serialize::serializable_range<std::wstring_view, wchar_t>);
static_assert(not serialize::serializable_range<char *>);
static_assert(not serialize::serializable_range<wchar_t *, wchar_t>);
static_assert(not serialize::serializable_range<char const *>);
static_assert(not serialize::serializable_range<wchar_t const *, wchar_t>);
static_assert(not serialize::serializable_range<std::filesystem::path>);
