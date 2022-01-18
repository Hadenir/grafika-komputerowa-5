#pragma once

#include <string>
#include <functional>

class GLFWwindow;

class Display
{
public:
    Display(const std::string& title, size_t width, size_t height);
    ~Display();

    [[nodiscard]] size_t get_width() const;
    [[nodiscard]] size_t get_height() const;

    [[nodiscard]] bool get_key(int key);

    [[nodiscard]] bool should_close() const;

    void close();

    void clear() const;

    void display() const;

    void set_keyboard_callback(std::function<void(int)> callback);

    void set_mouse_callback(std::function<void(float, float)> callback);

    void set_scroll_callback(std::function<void(float)> callback);

    Display(const Display&) = delete;
    void operator=(const Display&) = delete;

private:
    size_t width, height;

    bool glfw_initialized = false;
    GLFWwindow* window = nullptr;

    std::function<void(int)> keyboard_callback;
    std::function<void(float, float)> mouse_callback;
    std::function<void(float)> scroll_callback;

    bool mouse_initialized = false;
    float last_mouse_x = 0, last_mouse_y = 0;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos);
    static void mouse_scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
};

