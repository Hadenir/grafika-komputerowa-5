#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "display.hpp"

Display::Display(const std::string& title, size_t width, size_t height)
{
    if(glfwInit() != GLFW_TRUE)
        throw std::runtime_error("Failed to initialize GLFW!");
    glfw_initialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow((int)width, (int)height, title.c_str(), nullptr, nullptr);
    if (window == nullptr)
        throw std::runtime_error("Failed to create GLFW window!");
    this->width = width;
    this->height = height;
    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD!");

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

Display::~Display()
{
    if(window != nullptr)
        glfwDestroyWindow(window);

    if(glfw_initialized)
        glfwTerminate();
}

size_t Display::get_width() const
{
    return width;
}

size_t Display::get_height() const
{
    return height;
}

GLFWwindow* Display::get_glfw_window() const
{
    return window;
}

[[maybe_unused]] bool Display::should_close() const
{
    return glfwWindowShouldClose(window);
}

void Display::clear() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Display::display() const
{
    glfwPollEvents();
    glfwSwapBuffers(window);
}

void Display::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Display& display = *(Display*)glfwGetWindowUserPointer(window);
    display.width = width;
    display.height = height;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}
