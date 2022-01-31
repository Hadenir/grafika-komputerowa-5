#include "scene.hpp"

Scene::Scene(Camera& camera)
    : camera(camera)
{
    setup_scene();
}

void Scene::update(float delta_time)
{
    spot_lights[0].position = camera.get_position();
    spot_lights[0].direction = camera.get_direction();
}

void Scene::draw(PhongShader& shader) const
{
    shader.set_mat4("viewMat", camera.get_view_matrix());
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

    shader.set_vec3("viewPos", camera.get_position());

    for(const RenderObject& object: objects)
        object.draw(shader);
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

    // models.push_back(std::make_unique<Model>("resources/models/vazz/Vazz.obj"));
    models.push_back(std::make_unique<Model>("resources/models/building1/untitled.obj"));
    models.push_back(std::make_unique<Model>("resources/models/asphalt/asphalt.obj"));
    // models.push_back(std::make_unique<Model>("resources/models/building2/Farm_house.obj"));

    // std::vector<std::unique_ptr<Texture>> textures;
    // textures.emplace_back(std::make_unique<Texture>("resources/models/asphalt/white.png"));
    // std::vector<Vertex> vertices = {
    //     Vertex(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    //     Vertex(glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    //     Vertex(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    //     Vertex(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f))
    // };
    // std::vector<unsigned int> indices = {0, 1, 3, 3, 2, 0};
    // std::vector<Mesh> meshes;
    // meshes.emplace_back(
    //     Mesh(
    //         std::move(vertices),
    //         std::move(indices),
    //         Material(
    //             glm::vec3(1.0f, 1.0f, 1.0f),
    //             glm::vec3(1.0f, 1.0f, 1.0f),
    //             glm::vec3(1.0f, 1.0f, 1.0f),
    //             0.0f, {textures[0].get()})));
    //
    // models.emplace_back(std::make_unique<Model>(std::move(meshes), std::move(textures)));

    objects.emplace_back(
        models[0].get(),
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.5f));
    objects.emplace_back(
        models[1].get(),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f));

    directional_lights.emplace_back(
        glm::vec3(0.05f, 0.05f, 0.05f),
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
}
