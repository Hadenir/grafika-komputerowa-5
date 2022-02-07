#define GLFW_INCLUDE_NONE

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "scene.hpp"
#include "imgui.h"

Scene::Scene(Camera& camera)
    : camera(camera),
      phong_shader(read_shader("resources/shaders/phong.vert"), read_shader("resources/shaders/phong.frag")),
      gouraud_shader(read_shader("resources/shaders/gouraud.vert"), read_shader("resources/shaders/gouraud.frag"))
{
    setup_scene();
}

void Scene::update(const Display& display, float delta_time)
{
    ImGui::Begin("Window");
    ImGui::Text("Camera Type:");
    if(ImGui::RadioButton("Free Camera", &camera_type_id, 0))
    {
        camera.set_position({0.0f, 3.0f, 10.0f});
        camera.set_rotation(0.0f, 0.0f);
        camera_speed = 2.0f;
    }
    if(ImGui::RadioButton("Chase Camera", &camera_type_id, 1))
    {
        camera.set_position({0.0f, 40 * 3.0f, 40 * -5.0f});
        camera.set_rotation(-10.0f, 90.0f);
        camera_speed = 40.0f;
    }
    ImGui::Text("Fog:");
    ImGui::SliderFloat("Fog Density", &fog_density, 0.001f, 0.05f);
    ImGui::SliderFloat("Fog Gradient", &fog_gradient, 1.0f, 10.0f);
    ImGui::Text("Reflectors:");
    ImGui::SliderFloat("Reflectors Angle", &reflectors_angle, -0.5f, 0.5f);
    ImGui::SliderFloat("Reflectors Spread", &reflectors_spread, 9.5f, 20.0f);
    ImGui::Text("Shading:");
    ImGui::RadioButton("Phong", &shader_type, 0);
    ImGui::RadioButton("Gouraud", &shader_type, 1);
    ImGui::End();

    if(display.get_key(GLFW_KEY_UP))
        camera.move(Direction::Forward, camera_speed * delta_time);
    if(display.get_key(GLFW_KEY_DOWN))
        camera.move(Direction::Backward, camera_speed * delta_time);
    if(display.get_key(GLFW_KEY_RIGHT))
        camera.move(Direction::Right, camera_speed * delta_time);
    if(display.get_key(GLFW_KEY_LEFT))
        camera.move(Direction::Left, camera_speed * delta_time);
    if(display.get_key(GLFW_KEY_SPACE))
        camera.move(Direction::Up, camera_speed * delta_time);
    if(display.get_key(GLFW_KEY_LEFT_SHIFT))
        camera.move(Direction::Down, camera_speed * delta_time);
    if(display.get_key(GLFW_KEY_A))
        car->rotate({0.0f, 0.2f * delta_time, 0.0f});
    if(display.get_key(GLFW_KEY_D))
        car->rotate({0.0f, -0.2f * delta_time, 0.0f});
    if(display.get_key(GLFW_KEY_W))
        car->move(car->get_front_dir() * delta_time * 4.0f);
    if(display.get_key(GLFW_KEY_S))
        car->move(car->get_front_dir() * delta_time * -4.0f);

    if(camera_type_id == 1)
    {
        glm::mat4 car_model_mat = car->get_model_matrix();
        glm::mat4 car_rotation_mat = car->get_rotation_matrix();
        glm::vec3 camera_pos = car_model_mat * glm::vec4(camera.get_position(), 1.0);
        glm::vec3 camera_dir = car_rotation_mat * glm::vec4(camera.get_direction(), 1.0f);
        spot_lights[2].position = camera_pos;
        spot_lights[2].direction = camera_dir;
    }
    else
    {
        spot_lights[2].position = camera.get_position();
        spot_lights[2].direction = camera.get_direction();
    }

    spot_lights[0].direction.y = reflectors_angle;
    spot_lights[0].outer_cutoff = glm::cos(glm::radians(reflectors_spread));
    spot_lights[1].direction.y = reflectors_angle;
    spot_lights[1].outer_cutoff = glm::cos(glm::radians(reflectors_spread));
}

void Scene::draw()
{
    PhongShader& shader = shader_type == 0 ? phong_shader : gouraud_shader;
    shader.use();

    glm::mat4 car_model_mat = car->get_model_matrix();
    glm::mat4 car_rotation_mat = car->get_rotation_matrix();
    glm::vec3 camera_pos = car_model_mat * glm::vec4(camera.get_position(), 1.0);

    if(camera_type_id == 1)
    {
        shader.set_mat4("viewMat", camera.get_view_matrix(car_model_mat, car->get_rotation_matrix()));
        shader.set_vec3("viewPos", camera_pos);
    }
    else
    {
        shader.set_mat4("viewMat", camera.get_view_matrix());
        shader.set_vec3("viewPos", camera.get_position());
    }

    shader.set_mat4("projMat", camera.get_projection_matrix());

    shader.set_int("numDirLights", (int) directional_lights.size());
    for(auto i = 0; i < directional_lights.size(); i++)
        shader.set_directional_light(i, directional_lights[i]);

    shader.set_int("numPointLights", (int) point_lights.size());
    for(auto i = 0; i < point_lights.size(); i++)
        shader.set_point_light(i, point_lights[i]);

    shader.set_int("numSpotLights", (int) spot_lights.size());
    for(auto i = 2; i < spot_lights.size(); i++)
        shader.set_spot_light(i, spot_lights[i]);

    SpotLight reflector1 = spot_lights[0];
    reflector1.position = car_model_mat * glm::vec4(reflector1.position, 1.0f);
    reflector1.direction = car_rotation_mat * glm::vec4(reflector1.direction, 1.0f);
    shader.set_spot_light(0, reflector1);
    SpotLight reflector2 = spot_lights[1];
    reflector2.position = car_model_mat * glm::vec4(reflector2.position, 1.0f);
    reflector2.direction = car_rotation_mat * glm::vec4(reflector2.direction, 1.0f);
    shader.set_spot_light(1, reflector2);

    shader.set_vec4("skyColor", sky_color);
    shader.set_float("fogDensity", fog_density);
    shader.set_float("fogGradient", fog_gradient);

    for(const auto& object: objects)
        object->draw(shader);
}

void Scene::setup_scene()
{
    // Model model("resources/models/backpack/backpack.obj");
    // Model model("resources/models/teapot/teapot.obj");
    // models.push_back(std::make_unique<Model>("resources/models/asphalt/asphalt.obj"));
    // RenderObject building(
    //         models[0].get(),
    //         glm::vec3(0.0f, 0.0f, 0.0f),
    //         glm::vec3(0.0f, 0.0f, 0.0f),
    //         glm::vec3(0.2f, 0.2f, 0.2f));

    Model* vazz_model = models.emplace_back(std::make_unique<Model>("resources/models/vazz/Vazz.obj")).get();
    Model* building1_model = models.emplace_back(std::make_unique<Model>("resources/models/building1/untitled.obj")).get();
    Model* building2_model = models.emplace_back(std::make_unique<Model>("resources/models/building2/untitled.obj")).get();
    Model* asphalt_model = models.emplace_back(std::make_unique<Model>("resources/models/asphalt/asphalt.obj")).get();
    Model* lamppost_model = models.emplace_back(std::make_unique<Model>("resources/models/lamppost/street lamp.obj")).get();

    spot_lights.emplace_back(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(30.0f, 10.0f, 60.0f),
        1.0f, 0.014f, 0.0007f,
        glm::vec3(0.0f, -0.05f, 1.0f),
        glm::cos(glm::radians(9.5f)),
        glm::cos(glm::radians(10.0f)));
    spot_lights.emplace_back(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(-30.0f, 10.0f, 60.0f),
        1.0f, 0.014f, 0.0007f,
        glm::vec3(0.0f, -0.05f, 1.0f),
        glm::cos(glm::radians(9.5f)),
        glm::cos(glm::radians(10.0f)));

    directional_lights.emplace_back(
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(-0.2f, -1.0f, -0.3f));
    point_lights.emplace_back(
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.7f, 0.2f, 2.0f),
        1.0f, 0.09f, 0.032f);
    spot_lights.emplace_back(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 10.0f, 0.0f),
        1.0f, 0.09f, 0.032f,
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::cos(glm::radians(12.5f)),
        glm::cos(glm::radians(15.0f)));

    car = objects.emplace_back(std::make_unique<RenderObject>(
        vazz_model,
        glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(0.025f, 0.025f, 0.025f))).get();

    for(int j = -15; j <= 15; j++)
    {
        for(int i = -15; i <= 15; i++)
        {
            float x = (float) i;
            float z = (float) j;

            objects.emplace_back(std::make_unique<RenderObject>(asphalt_model, glm::vec3(x * 5.0f, 0.0f, z * 5.0f)));

            if(j % 4 == 0)
            {
                if(i % 4 == 0)
                    objects.emplace_back(std::make_unique<RenderObject>(
                        building1_model,
                        glm::vec3(x * 5.0f, 0.0f, z * 5.0f),
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.5f, 0.5f, 0.5f)));
                else if(i % 4 == 1 || i % 4 == -3)
                    objects.emplace_back(std::make_unique<RenderObject>(
                        building2_model,
                        glm::vec3(x * 5.0f + 2.5f, 0.0f, z * 5.0f),
                        glm::vec3(0.0f, -1.0f, 0.0f)));
            }
            else if((j % 4 == 1 || j % 4 == -3) && (i % 4 == 1 || i % 4 == -3))
            {
                objects.emplace_back(std::make_unique<RenderObject>(
                    lamppost_model,
                    glm::vec3(x * 5.0f, 0.0f, z * 5.0f),
                    glm::vec3(0.0f, 0.5f, 0.0f)));
                spot_lights.emplace_back(
                    glm::vec3(0.1f, 0.1f, 0.1f),
                    glm::vec3(0.8f, 0.8f, 0.8f),
                    glm::vec3(1.0f, 1.0f, 1.0f),
                    glm::vec3(x * 5.0f, 5.0f, z * 5.0f),
                    1.0f, 0.09f, 0.032f,
                    glm::vec3(0.0f, -1.0f, 0.1f),
                    glm::cos(glm::radians(28.0f)),
                    glm::cos(glm::radians(30.0f)));
            }
        }
    }
}

std::string Scene::read_shader(const std::string& path)
{
    std::ifstream file(path);
    if(file.fail())
        throw std::runtime_error("Could not open file!");

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
