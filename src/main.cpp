#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <imgui.h>
#include <fstream>
#include <sstream>

#include "display/display.hpp"
#include "render/shader/shader.hpp"
#include "render/texture.hpp"
#include "render/camera.hpp"
#include "render/model.hpp"
#include "object/render_object.hpp"

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

    std::string vertex_source = read_shader("resources/shaders/phong.vert");
    std::string fragment_source = read_shader("resources/shaders/phong.frag");
    PhongShader shader(vertex_source, fragment_source);

    // Model model("resources/models/backpack/backpack.obj");
    // Model model("resources/models/teapot/teapot.obj");
    std::vector<std::unique_ptr<Model>> models;
    models.push_back(std::make_unique<Model>("resources/models/vazz/Vazz.obj"));
    // models.push_back(std::make_unique<Model>("resources/models/asphalt/asphalt.obj"));

    RenderObject vazz_car(
            models[0].get(),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.05f, 0.05f, 0.05f));
    // RenderObject building(
    //         models[0].get(),
    //         glm::vec3(0.0f, 0.0f, 0.0f),
    //         glm::vec3(0.0f, 0.0f, 0.0f),
    //         glm::vec3(0.2f, 0.2f, 0.2f));

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), display.get_width(), display.get_height());

    display.set_keyboard_callback([&](int key) { if(key == GLFW_KEY_ESCAPE) display.close(); });
    display.set_mouse_callback([&](float x, float y) { camera.rotate(x, -y); });
    display.set_scroll_callback([&](float off) { camera.zoom(off); });
    display.set_resize_callback([&](size_t width, size_t height) { camera.resize_viewport(width, height); });

    std::vector<DirectionalLight> directional_lights = {
            DirectionalLight(
                    glm::vec3(0.05f, 0.05f, 0.05f),
                    glm::vec3(0.4f, 0.4f, 0.4f),
                    glm::vec3(0.5f, 0.5f, 0.5f),
                    glm::vec3(-0.2f, -1.0f, -0.3f)),
    };

    std::vector<PointLight> point_lights = {
            PointLight(
                    glm::vec3(0.05f, 0.05f, 0.05f),
                    glm::vec3(0.8f, 0.8f, 0.8f),
                    glm::vec3(1.0f, 1.0f, 1.0f),
                    glm::vec3(0.7f, 0.2f, 2.0f),
                    1.0f, 0.09f, 0.032f),
    };

    std::vector<SpotLight> spot_lights = {
            SpotLight(
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f),
                    glm::vec3(0.0f, 10.0f, 0.0f),
                    1.0f, 0.09f, 0.032f,
                    glm::vec3(0.0f, -1.0f, 0.0f),
                    glm::cos(glm::radians(12.5f)),
                    glm::cos(glm::radians(15.0f))),
    };

    float delta_time;
    float last_frame = (float) glfwGetTime();
    while(!display.should_close())
    {
        float current_frame = (float) glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        display.begin_frame();

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

        shader.set_int("numDirLights", (int) directional_lights.size());
        for(auto i = 0; i < directional_lights.size(); i++)
            shader.set_directional_light(i, directional_lights[i]);

        shader.set_int("numPointLights", (int) point_lights.size());
        for(auto i = 0; i < point_lights.size(); i++)
            shader.set_point_light(i, point_lights[i]);

        spot_lights[0].position = camera.get_position();
        spot_lights[0].direction = camera.get_direction();

        shader.set_int("numSpotLights", (int) spot_lights.size());
        for(auto i = 0; i < spot_lights.size(); i++)
            shader.set_spot_light(i, spot_lights[i]);

        shader.set_vec3("viewPos", camera.get_position());
        vazz_car.draw(shader);

        // ImGui::ShowDemoWindow();

        display.end_frame();
    }

    return 0;
}
