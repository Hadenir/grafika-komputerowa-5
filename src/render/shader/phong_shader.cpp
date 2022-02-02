#include <stdexcept>

#include "phong_shader.hpp"

PhongShader::PhongShader(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
        : Shader(vertex_shader_source, fragment_shader_source)
{}

void PhongShader::set_material(const Material& material)
{
    set_vec3("material.ambient", material.ambient);
    set_vec3("material.diffuse", material.diffuse);
    set_vec3("material.specular", material.specular);
    set_float("material.shininess", material.shininess);
    set_int("material.textureDiffuse", 0);
    set_int("material.textureSpecular", 1);
}

void PhongShader::set_directional_light(size_t i, DirectionalLight light)
{
    if(i >= 10) throw std::runtime_error("Light index out of range");

    std::string base_name = "dirLights[" + std::to_string(i) + "]";
    set_vec3(base_name + ".ambient", light.ambient);
    set_vec3(base_name + ".diffuse", light.diffuse);
    set_vec3(base_name + ".specular", light.specular);
    set_vec3(base_name + ".direction", light.direction);
}

void PhongShader::set_point_light(size_t i, PointLight light)
{
    if(i >= 10) throw std::runtime_error("Light index out of range");

    std::string base_name = "pointLights[" + std::to_string(i) + "]";
    set_vec3(base_name + ".ambient", light.ambient);
    set_vec3(base_name + ".diffuse", light.diffuse);
    set_vec3(base_name + ".specular", light.specular);
    set_vec3(base_name + ".position", light.position);
    set_float(base_name + ".kConstant", light.constant);
    set_float(base_name + ".kLinear", light.linear);
    set_float(base_name + ".kQuadratic", light.quadratic);
}

void PhongShader::set_spot_light(size_t i, SpotLight light)
{
    if(i >= 90) throw std::runtime_error("Light index out of range");

    std::string base_name = "spotLights[" + std::to_string(i) + "]";
    set_vec3(base_name + ".ambient", light.ambient);
    set_vec3(base_name + ".diffuse", light.diffuse);
    set_vec3(base_name + ".specular", light.specular);
    set_vec3(base_name + ".position", light.position);
    set_float(base_name + ".kConstant", light.constant);
    set_float(base_name + ".kLinear", light.linear);
    set_float(base_name + ".kQuadratic", light.quadratic);
    set_vec3(base_name + ".direction", light.direction);
    set_float(base_name + ".innerCutoff", light.inner_cutoff);
    set_float(base_name + ".outerCutoff", light.outer_cutoff);
}
