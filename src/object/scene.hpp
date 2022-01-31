#pragma once

#include "render_object.hpp"
#include "../render/camera.hpp"

class Scene
{
public:
    explicit Scene(Camera& camera);

    void update(float delta_time);

    void draw(PhongShader& shader) const;

private:
    Camera& camera;

    std::vector<std::unique_ptr<Model>> models;
    std::vector<RenderObject> objects;
    std::vector<DirectionalLight> directional_lights;
    std::vector<PointLight> point_lights;
    std::vector<SpotLight> spot_lights;

    void setup_scene();
};

