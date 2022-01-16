#pragma once

#include <string>

class GLFWwindow;

class Display
{
public:
    Display(const std::string& title, size_t width, size_t height);
    ~Display();

    Display(const Display&) = delete;
    void operator=(const Display&) = delete;

    [[nodiscard]] size_t get_width() const;
    [[nodiscard]] size_t get_height() const;

    [[nodiscard]] GLFWwindow* get_glfw_window() const;

    [[nodiscard]] bool should_close() const;

    void clear() const;

    void display() const;

private:
    size_t width, height;

    bool glfw_initialized = false;
    GLFWwindow* window = nullptr;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

