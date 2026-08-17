#pragma once

#include <glfw/library.hpp>
#include <glfw/monitor.hpp>

#include <set>

namespace glfw
{
struct internal
{
    [[nodiscard]]
    static library init_library();

    static std::set<glfw::monitor>::iterator try_emplace_monitor(GLFWmonitor * glfw_monitor);

    static std::set<monitor> monitors;

    static void window_pos_callback(GLFWwindow * window, int xpos, int ypos);

    static void window_size_callback(GLFWwindow * window, int width, int height);

    static void window_close_callback(GLFWwindow * window);

    static void window_refresh_callback(GLFWwindow * window);

    static void window_focus_callback(GLFWwindow * window, int focused);

    static void window_iconify_callback(GLFWwindow * window, int iconified);

    static void window_maximize_callback(GLFWwindow * window, int maximized);

    static void framebuffer_size_callback(GLFWwindow * window, int width, int height);

    static void window_content_scale_callback(GLFWwindow * window, float xscale, float yscale);

    static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

    static void char_callback(GLFWwindow * window, unsigned int codepoint);

    static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);

    static void cursor_pos_callback(GLFWwindow * window, double xpos, double ypos);

    static void cursor_enter_callback(GLFWwindow * window, int entered);

    static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

    static void drop_callback(GLFWwindow * window, int path_count, char const ** paths);
};
}
