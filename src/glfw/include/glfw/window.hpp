#pragma once

#include <glfw/fwd.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <cuchar>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

namespace glfw
{
class window
{
  public:
    window(window && other) noexcept;

    window(window const &) noexcept = delete;

    window & operator=(window const &) noexcept = delete;

    window & operator=(window && other) noexcept;

    virtual ~window() = default;

    [[nodiscard]]
    std::strong_ordering operator<=>(window const & other) const noexcept;

    [[nodiscard]]
    std::string_view get_title() const;

    [[nodiscard]]
    coordinates<int, 2> get_pos() const;

    [[nodiscard]]
    dimensions<int, 2> get_size() const;

    [[nodiscard]]
    dimensions<int, 2> get_framebuffer_size() const;

    [[nodiscard]]
    edges<int> get_frame_size() const;

    [[nodiscard]]
    coordinates<float, 2> get_content_scale() const;

    [[nodiscard]]
    float get_opacity() const;

    [[nodiscard]]
    monitor const & get_monitor() const;

    [[nodiscard]]
    bool is_input_mode_cursor() const;

    [[nodiscard]]
    bool is_input_mode_sticky_keys() const;

    [[nodiscard]]
    bool is_input_mode_sticky_mouse_buttons() const;

    [[nodiscard]]
    bool is_input_mode_lock_key_mods() const;

    [[nodiscard]]
    bool is_input_mode_raw_mouse_motion() const;

    [[nodiscard]]
    action get_key(key const key) const;

    [[nodiscard]]
    action get_mouse_button(mouse_button const button) const;

    [[nodiscard]]
    coordinates<double, 2> get_cursor_pos() const;

    [[nodiscard]]
    vk::raii::SurfaceKHR create_surface(vk::raii::Instance const &                        instance,
                                        vk::Optional<vk::AllocationCallbacks const> const allocator = nullptr) const;

    [[nodiscard]]
    bool should_close() const;

    [[nodiscard]]
    bool is_focused() const;

    [[nodiscard]]
    bool is_minimized() const;

    [[nodiscard]]
    bool is_maximized() const;

    [[nodiscard]]
    bool is_cursor_hovered() const;

    [[nodiscard]]
    bool is_visible() const;

    [[nodiscard]]
    bool is_resizable() const;

    [[nodiscard]]
    bool is_decorated() const;

    [[nodiscard]]
    bool is_auto_minimized() const;

    [[nodiscard]]
    bool is_floating() const;

    [[nodiscard]]
    bool is_transparent_framebuffer() const;

    [[nodiscard]]
    bool is_focus_on_show() const;

    void set_title(std::string const & title);

    void set_icon(std::span<image> const images);

    void set_pos(coordinates<int, 2> const & pos);

    void set_size_limits(dimensions<int, 2> const & min, dimensions<int, 2> const & max);

    void set_aspect_ratio(int const numer, int const denom);

    void set_size(dimensions<int, 2> const & size);

    void set_monitor(monitor const & monitor, workarea const & area, int const refreshRate);

    void set_input_mode_cursor(bool const value);

    void set_input_mode_sticky_keys(bool const value);

    void set_input_mode_sticky_mouse_buttons(bool const value);

    void set_input_mode_lock_key_mods(bool const value);

    void set_input_mode_raw_mouse_motion(bool const value);

    void set_cursor_pos(coordinates<double, 2> const & pos);

    void set_cursor(cursor const & cursor);

    void set_clipboard_string(std::string const & str);

    void set_should_close(bool const value);

    void set_opacity(float const opacity);

    void minimize();

    void restore();

    void maximize();

    void show();

    void hide();

    void focus();

    void request_attention();

    virtual void close() noexcept;

  protected:
    virtual void when_window_moved(coordinates<int, 2> const &);

    virtual void when_window_resized(dimensions<int, 2> const &);

    virtual void when_window_closed();

    virtual void when_window_refreshed();

    virtual void when_window_focused();

    virtual void when_window_unfocused();

    virtual void when_window_minimized();

    virtual void when_window_unminimized();

    virtual void when_window_maximized();

    virtual void when_window_unmaximized();

    virtual void when_framebuffer_resized(dimensions<int, 2> const &);

    virtual void when_window_content_scaled(coordinates<float, 2> const &);

    virtual void when_key_pressed(key const, int const, action const, modifier const);

    virtual void when_unicode_char_typed(char32_t const);

    virtual void when_mouse_button_pressed(mouse_button const, action const, modifier const);

    virtual void when_cursor_moved(coordinates<double, 2> const &);

    virtual void when_cursor_entered();

    virtual void when_cursor_exited();

    virtual void when_mouse_scrolled(coordinates<double, 2> const &);

    virtual void when_file_dropped(std::span<std::filesystem::path const> const);

  private:
    friend struct internal;

    friend library;

    explicit window(std::unique_ptr<GLFWwindow, void (*)(GLFWwindow *)> && new_ptr) noexcept;

    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow *)> ptr;
};
}
