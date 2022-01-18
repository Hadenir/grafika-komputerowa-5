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
#include "render/model.hpp"

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

    std::string vertex_source = read_shader("resources/shaders/vertex.vert");
    std::string fragment_source = read_shader("resources/shaders/fragment.frag");
    Shader shader(vertex_source, fragment_source);

    Model model("resources/models/backpack/backpack.obj");

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

        glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.set_mat4("modelMat", model_matrix);
        model.draw(shader);

        display.display();
    }

    return 0;
}
