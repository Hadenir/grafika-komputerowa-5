#pragma once

#include "shader.hpp"
#include "../material.hpp"
#include "../light.hpp"

class PhongShader : public Shader
{
public:
    PhongShader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);

    void set_material(const Material& material);
    void set_directional_light(size_t i, DirectionalLight light);
    void set_point_light(size_t i, PointLight light);
    void set_spot_light(size_t i, SpotLight light);
};

