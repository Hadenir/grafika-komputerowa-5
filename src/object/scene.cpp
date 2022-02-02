#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "scene.hpp"
#include "imgui.h"

Scene::Scene(Camera& camera)
    : camera(camera)
{
    setup_scene();
}

void Scene::update(const Display& display, float delta_time)
{
    // ImGui::ShowDemoWindow();

    ImGui::Begin("Window");
    ImGui::Text("Camera Type:");
    if(ImGui::RadioButton("Free Camera", &camera_type_id, 0))
    {
        camera.set_position({0.0f, 3.0f, 10.0f});
        camera.set_rotation(0.0f, 0.0f);
        camera_speed = 1.0f;
    }
    if(ImGui::RadioButton("Chase Camera", &camera_type_id, 1))
    {
        camera.set_position({0.0f, 40 * 3.0f, 40 * -5.0f});
        camera.set_rotation(-10.0f, 90.0f);
        camera_speed = 40.0f;
    }
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
        spot_lights[0].position = camera_pos;
        spot_lights[0].direction = camera_dir;
    }
    else
    {
        spot_lights[0].position = camera.get_position();
        spot_lights[0].direction = camera.get_direction();
    }
}

void Scene::draw(PhongShader& shader) const
{
    glm::mat4 car_model_mat = car->get_model_matrix();
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
    for(auto i = 0; i < spot_lights.size(); i++)
        shader.set_spot_light(i, spot_lights[i]);

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

    for(int j = -12; j <= 12; j++)
    {
        for(int i = -12; i <= 12; i++)
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
