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
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
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

    GLsizei vertex_attribs_size = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_attribs_size, nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_attribs_size, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_attribs_size, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_attribs_size, nullptr);
    glEnableVertexAttribArray(0);

    Texture diffuse_texture("resources/textures/container2.png", GL_TEXTURE0);
    diffuse_texture.bind();
    Texture specular_texture("resources/textures/container2_specular.png", GL_TEXTURE1);
    specular_texture.bind();

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
        shader.set_int("material.diffuse", 0);
        shader.set_int("material.specular", 1);
        shader.set_float("material.shininess", 32.0f);

        shader.set_vec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader.set_vec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        shader.set_vec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        shader.set_vec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

        shader.set_vec3("pointLight.position", glm::vec3(0.7f, 0.2f, 2.0f));
        shader.set_float("pointLight.kConstant", 1.0f);
        shader.set_float("pointLight.kLinear", 0.09f);
        shader.set_float("pointLight.kQuadratic", 0.032f);
        shader.set_vec3("pointLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        shader.set_vec3("pointLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        shader.set_vec3("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.set_vec3("spotLight.position", camera.get_position());
        shader.set_vec3("spotLight.direction", camera.get_direction());
        shader.set_float("spotLight.innerCutoff", glm::cos(glm::radians(12.5f)));
        shader.set_float("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));
        shader.set_float("spotLight.kConstant", 1.0f);
        shader.set_float("spotLight.kLinear", 0.09f);
        shader.set_float("spotLight.kQuadratic", 0.032f);
        shader.set_vec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.set_vec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.set_vec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.set_vec3("viewPos", camera.get_position());

        glBindVertexArray(VAO);
        for(auto i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            float angle = 20.0f * (float)i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.set_mat4("modelMat", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
