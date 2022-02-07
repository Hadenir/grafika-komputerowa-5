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

    void draw();

private:
    Camera& camera;

    std::vector<std::unique_ptr<Model>> models;
    std::vector<std::unique_ptr<RenderObject>> objects;
    std::vector<DirectionalLight> directional_lights;
    std::vector<PointLight> point_lights;
    std::vector<SpotLight> spot_lights;

    RenderObject* car = nullptr;

    int shader_type = 0;
    PhongShader phong_shader;
    PhongShader gouraud_shader;

    int camera_type_id = 1;
    float camera_speed = 40;
    glm::vec4 sky_color = glm::vec4 (0.53f, 0.81f, 0.92f, 1.0f);
    float fog_density = 0.02;
    float fog_gradient = 3.0;
    float reflectors_angle = -0.05f;
    float reflectors_spread = 10.0f;

    void setup_scene();

    static std::string read_shader(const std::string& path);
};

