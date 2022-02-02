#pragma once

#include <glm/glm.hpp>

#include "render_object.hpp"
#include "../display/display.hpp"
#include "../render/camera.hpp"

class Scene
{
public:
    explicit Scene(Camera& camera);

    void update(const Display& display, float delta_time);

    void draw(PhongShader& shader) const;

private:
    Camera& camera;

    std::vector<std::unique_ptr<Model>> models;
    std::vector<std::unique_ptr<RenderObject>> objects;
    std::vector<DirectionalLight> directional_lights;
    std::vector<PointLight> point_lights;
    std::vector<SpotLight> spot_lights;

    RenderObject* car = nullptr;

    int camera_type_id = 1;
    float camera_speed = 40;
    glm::vec4 sky_color = glm::vec4 (0.53f, 0.81f, 0.92f, 1.0f);
    float fog_density = 0.02;
    float fog_gradient = 3.0;

    void setup_scene();
};

