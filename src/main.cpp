#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "display/display.hpp"
#include "render/shader.hpp"

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

int main()
{
    Display display("LearnOpenGL", 800, 600);
    GLFWwindow* window = display.get_glfw_window();

    float vertices[] = {
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    unsigned int indices[] = {
            0, 1, 2,
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::string vertex_source = read_shader("shaders/vertex.vert");
    std::string fragment_source = read_shader("shaders/fragment.frag");

    Shader shader(vertex_source, fragment_source);
    shader.use();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while(!display.should_close())
    {
        display.clear();

        processInput(window);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        display.display();
    }

    return 0;
}
