#pragma once

#include <glfw/def.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <cuchar>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

typedef struct GLFWwindow GLFWwindow;

namespace glfw
{

class window
{
  public:
    window(window && other) noexcept;

    window(window const &) noexcept = delete;

    window & operator=(window const &) noexcept = delete;

    window & operator=(window && other) noexcept;

    ~window() = default;

    [[nodiscard]]
    std::strong_ordering operator<=>(window const & other) const noexcept;

    [[nodiscard]]
    bool should_close() const;

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
    class monitor const & get_monitor() const;

    void set_monitor(class monitor const & monitor, workarea area, int refreshRate);

    [[nodiscard]]
    bool get_input_mode(input_mode mode) const;

    [[nodiscard]]
    action get_key(key key) const;

    [[nodiscard]]
    action get_mouse_button(mouse_button button) const;

    [[nodiscard]]
    coordinates<double, 2> get_cursor_pos() const;

    [[nodiscard]]
    vk::raii::SurfaceKHR create_surface(vk::raii::Instance const &                  instance,
                                        vk::Optional<vk::AllocationCallbacks const> allocator = nullptr) const;

    void set_title(std::string const & title);

    void set_icon(std::span<image> images);

    void set_pos(coordinates<int, 2> pos);

    void set_size_limits(dimensions<int, 2> min, dimensions<int, 2> max);

    void set_aspect_ratio(int numer, int denom);

    void set_size(dimensions<int, 2> size);

    void set_input_mode(input_mode mode, bool value);

    void set_cursor_pos(coordinates<double, 2> pos);

    void set_cursor(class cursor const & cursor);

    void set_clipboard_string(std::string const & str);

    void set_should_close(bool value);

    void set_opacity(float opacity);

    void minimize();

    void restore();

    void maximize();

    void show();

    void hide();

    void focus();

    void request_attention();

    std::function<void(window &, coordinates<int, 2>)>                    when_window_moved;
    std::function<void(window &, dimensions<int, 2>)>                     when_window_resized;
    std::function<void(window &)>                                         when_window_closed;
    std::function<void(window &)>                                         when_window_refreshed;
    std::function<void(window &)>                                         when_window_focused;
    std::function<void(window &)>                                         when_window_unfocused;
    std::function<void(window &)>                                         when_window_minimized;
    std::function<void(window &)>                                         when_window_unminimized;
    std::function<void(window &)>                                         when_window_maximized;
    std::function<void(window &)>                                         when_window_unmaximized;
    std::function<void(window &, coordinates<int, 2>)>                    when_framebuffer_resized;
    std::function<void(window &, coordinates<float, 2>)>                  when_window_content_scaled;
    std::function<void(window &, key, int, action, modifier)>             when_key_pressed;
    std::function<void(window &, char32_t)>                               when_unicode_char_typed;
    std::function<void(window &, mouse_button, action, modifier)>         when_mouse_button_pressed;
    std::function<void(window &, coordinates<double, 2>)>                 when_cursor_moved;
    std::function<void(window &)>                                         when_cursor_entered;
    std::function<void(window &)>                                         when_cursor_exited;
    std::function<void(window &, coordinates<double, 2>)>                 when_mouse_scrolled;
    std::function<void(window &, std::span<std::filesystem::path const>)> when_file_dropped;

  private:
    friend class library;

    explicit window(std::unique_ptr<GLFWwindow, void (*)(GLFWwindow *)> && ptr) noexcept;

    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow *)> ptr;
};
}
