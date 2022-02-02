#include <stdexcept>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "display.hpp"

Display::Display(const std::string& title, size_t width, size_t height)
{
    if(glfwInit() != GLFW_TRUE)
        throw std::runtime_error("Failed to initialize GLFW!");
    glfw_initialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow((int)width, (int)height, title.c_str(), nullptr, nullptr);
    if (window == nullptr)
        throw std::runtime_error("Failed to create GLFW window!");
    this->width = width;
    this->height = height;
    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD!");

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

Display::~Display()
{
    if(window != nullptr)
        glfwDestroyWindow(window);

    if(glfw_initialized)
        glfwTerminate();

    ImGui::DestroyContext();
}

size_t Display::get_width() const
{
    return width;
}

size_t Display::get_height() const
{
    return height;
}

bool Display::get_key(int key) const
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Display::get_mouse_button(int button) const
{
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

bool Display::should_close() const
{
    return glfwWindowShouldClose(window);
}

void Display::close()
{
    glfwSetWindowShouldClose(window, true);
}

void Display::begin_frame() const
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Display::end_frame() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void Display::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Display& display = *(Display*)glfwGetWindowUserPointer(window);
    display.width = width;
    display.height = height;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    if(display.resize_callback)
        display.resize_callback(width, height);
}

void Display::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Display& display = *(Display*)glfwGetWindowUserPointer(window);

    if(action == GLFW_PRESS && display.keyboard_callback)
        display.keyboard_callback(key);
}

void Display::cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    Display& display = *(Display*)glfwGetWindowUserPointer(window);

    if(!display.mouse_initialized)
    {
        display.last_mouse_x = (float)x_pos;
        display.last_mouse_y = (float)y_pos;

        display.mouse_initialized = true;
        return;
    }

    float x_offset = (float)x_pos - display.last_mouse_x;
    float y_offset = (float)y_pos - display.last_mouse_y;
    display.last_mouse_x = (float)x_pos;
    display.last_mouse_y = (float)y_pos;

    if(display.mouse_callback)
        display.mouse_callback(x_offset, y_offset);
}

void Display::mouse_scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    Display& display = *(Display*)glfwGetWindowUserPointer(window);

    if(display.scroll_callback)
        display.scroll_callback((float)y_offset);
}

void Display::set_keyboard_callback(std::function<void(int)> callback)
{
    keyboard_callback = std::move(callback);
}

void Display::set_mouse_callback(std::function<void(float, float)> callback)
{
    mouse_callback = std::move(callback);
}

void Display::set_scroll_callback(std::function<void(float)> callback)
{
    scroll_callback = std::move(callback);
}

void Display::set_resize_callback(std::function<void(size_t, size_t)> callback)
{
    resize_callback = std::move(callback);
}
