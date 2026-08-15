#pragma once

#include <glfw/def.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <functional>
#include <set>
#include <string>

namespace glfw
{
extern library default_library;

class library
{
  public:
    library(library const &) = delete;

    library & operator=(library const &) = delete;

    ~library();

    [[nodiscard]]
    window create_window(dimensions<int, 2> const & size, std::string const & title) const;

    [[nodiscard]]
    window create_window(dimensions<int, 2> const & size,
                         std::string const &        title,
                         window const &             share) const;

    [[nodiscard]]
    window create_window(dimensions<int, 2> const & size,
                         std::string const &        title,
                         monitor const &            monitor) const;

    [[nodiscard]]
    window create_window(dimensions<int, 2> const & size,
                         std::string const &        title,
                         window const &             share,
                         monitor const &            monitor) const;

    [[nodiscard]]
    std::set<monitor> const & get_monitors() const;

    [[nodiscard]]
    monitor const & get_primary_monitor() const;

    [[nodiscard]]
    cursor create_cursor(image const & image, coordinates<int, 2> const & hotspot) const;

    [[nodiscard]]
    cursor create_arrow_cursor() const;

    [[nodiscard]]
    cursor create_ibeam_cursor() const;

    [[nodiscard]]
    cursor create_crosshair_cursor() const;

    [[nodiscard]]
    cursor create_pointing_hand_cursor() const;

    [[nodiscard]]
    cursor create_resize_ew_cursor() const;

    [[nodiscard]]
    cursor create_resize_ns_cursor() const;

    [[nodiscard]]
    cursor create_resize_nwse_cursor() const;

    [[nodiscard]]
    cursor create_resize_nesw_cursor() const;

    [[nodiscard]]
    cursor create_resize_all_cursor() const;

    [[nodiscard]]
    cursor create_not_allowed_cursor() const;

    [[nodiscard]]
    std::span<char const * const> get_required_instance_extensions() const;

    [[nodiscard]]
    bool get_physical_device_presentation_support(vk::raii::Instance const &       instance,
                                                  vk::raii::PhysicalDevice const & physical_device,
                                                  std::uint32_t const              queue_family) const;

    void poll_events() const;

    void wait_events() const;

    std::function<void(monitor const &)>  when_monitor_connected;
    std::function<void(std::string_view)> when_monitor_disconnected;

  private:
    friend struct internal;

    library();

    library(library &&) = default;

    library & operator=(library &&) = default;
};
}