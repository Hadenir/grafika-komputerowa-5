#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "display/display.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "render/camera.hpp"

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

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    std::string vertex_source = read_shader("resources/shaders/vertex.vert");
    std::string fragment_source = read_shader("resources/shaders/fragment.frag");
    std::string light_source = read_shader("resources/shaders/light.frag");

    Shader shader(vertex_source, fragment_source);
    Shader light_shader(vertex_source, light_source);

    GLsizei vertex_attribs_size = 6 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_attribs_size, nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_attribs_size, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_attribs_size, nullptr);
    glEnableVertexAttribArray(0);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), display.get_width(), display.get_height());

    display.set_keyboard_callback([&](int key) { if(key == GLFW_KEY_ESCAPE) display.close(); });
    display.set_mouse_callback([&](float x, float y) { camera.rotate(x, -y); });
    display.set_scroll_callback([&](float off) { camera.zoom(off); std::cout << off << std::endl; });

    glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

    float delta_time;
    float last_frame = (float)glfwGetTime();
    while(!display.should_close())
    {
        display.clear();

        float current_frame = (float)glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        if(display.get_key(GLFW_KEY_W))
            camera.move(Direction::Forward, delta_time);
        if(display.get_key(GLFW_KEY_S))
            camera.move(Direction::Backward, delta_time);
        if(display.get_key(GLFW_KEY_D))
            camera.move(Direction::Right, delta_time);
        if(display.get_key(GLFW_KEY_A))
            camera.move(Direction::Left, delta_time);
        if(display.get_key(GLFW_KEY_SPACE))
            camera.move(Direction::Up, delta_time);
        if(display.get_key(GLFW_KEY_LEFT_SHIFT))
            camera.move(Direction::Down, delta_time);

        shader.use();
        shader.set_mat4("viewMat", camera.get_view_matrix());
        shader.set_mat4("projMat", camera.get_projection_matrix());
        shader.set_mat4("modelMat", glm::mat4(1.0f));
        shader.set_vec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        shader.set_vec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.set_vec3("lightPos", light_pos);
        shader.set_vec3("viewPos", camera.get_position());

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        light_shader.use();
        light_shader.set_mat4("viewMat", camera.get_view_matrix());
        light_shader.set_mat4("projMat", camera.get_projection_matrix());
        glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), light_pos);
        model_mat = glm::scale(model_mat, glm::vec3(0.2f));
        light_shader.set_mat4("modelMat", model_mat);

        glBindVertexArray(light_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        display.display();
    }

    return 0;
}
