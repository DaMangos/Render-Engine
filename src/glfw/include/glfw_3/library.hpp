#pragma once

#include <glfw_3/def.hpp>

#include <functional>
#include <set>
#include <string>

namespace glfw_3
{
extern class library default_library;

class library
{
  public:
    library(library const &) = delete;

    library & operator=(library const &) = delete;

    ~library();

    [[nodiscard]]
    class window create_window(int2 size, std::string const & title) const;

    [[nodiscard]]
    class window create_window(int2 size, std::string const & title, class window const & share) const;

    [[nodiscard]]
    class window create_window(int2 size, std::string const & title, class monitor const & monitor) const;

    [[nodiscard]]
    class window create_window(int2                  size,
                               std::string const &   title,
                               class window const &  share,
                               class monitor const & monitor) const;

    [[nodiscard]]
    std::set<class monitor> const & get_monitors() const;

    [[nodiscard]]
    class monitor const & get_primary_monitor() const;

    [[nodiscard]]
    class cursor create_cursor(image image, int2 hotspot) const;

    [[nodiscard]]
    class cursor create_arrow_cursor() const;

    [[nodiscard]]
    class cursor create_ibeam_cursor() const;

    [[nodiscard]]
    class cursor create_crosshair_cursor() const;

    [[nodiscard]]
    class cursor create_pointing_hand_cursor() const;

    [[nodiscard]]
    class cursor create_resize_ew_cursor() const;

    [[nodiscard]]
    class cursor create_resize_ns_cursor() const;

    [[nodiscard]]
    class cursor create_resize_nwse_cursor() const;

    [[nodiscard]]
    class cursor create_resize_nesw_cursor() const;

    [[nodiscard]]
    class cursor create_resize_all_cursor() const;

    [[nodiscard]]
    class cursor create_not_allowed_cursor() const;

    [[nodiscard]]
    std::span<char const * const> get_required_instance_extensions() const;

    void poll_events() const;

    std::function<void(monitor const &)> when_monitor_connected;
    std::function<void(monitor const &)> when_monitor_disconnected;

  private:
    friend class internal;

    library();

    library(library &&) = default;

    library & operator=(library &&) = default;
};
}