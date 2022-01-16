#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "display/display.hpp"

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::string read_shader(const std::string& path)
{
    std::ifstream file(path);
    if(file.fail())
        throw std::runtime_error("Could not open file!");

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint create_shader(const std::string& shader_source, GLenum shader_type)
{
    const char* vertex_source_str = shader_source.c_str();
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &vertex_source_str, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char info[512];
        glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
        std::stringstream ss;
        ss << "Failed to compile shader: " << info << "!";
        throw std::runtime_error(ss.str());
    }

    return shader;
}

GLuint create_shader_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success)
    {
        char info[512];
        glGetProgramInfoLog(shader_program, sizeof(info), nullptr, info);
        std::stringstream ss;
        ss << "Failed to link shader program: " << info << "!";
        throw std::runtime_error(ss.str());
    }

    return shader_program;
}

int main()
{
    Display display("LearnOpenGL", 800, 600);
    GLFWwindow* window = display.get_glfw_window();

    float vertices[] = {
            -0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    std::string vertex_source = read_shader("shaders/vertex.vert");
    GLuint vertex_shader = create_shader(vertex_source, GL_VERTEX_SHADER);
    std::string fragment_source = read_shader("shaders/fragment.frag");
    GLuint fragment_shader = create_shader(fragment_source, GL_FRAGMENT_SHADER);

    GLuint shader_program = create_shader_program(vertex_shader, fragment_shader);
    glUseProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    while(!display.should_close())
    {
        display.clear();

        processInput(window);

        display.display();
    }

    return 0;
}
