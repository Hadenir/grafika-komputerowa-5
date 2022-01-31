#pragma once

#include "glm/glm.hpp"

struct Light
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : ambient(ambient), diffuse(diffuse), specular(specular)
    {}
};

struct DirectionalLight : public Light
{
    glm::vec3 direction;

    DirectionalLight(
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            glm::vec3 direction)
            : direction(direction), Light(ambient, diffuse, specular)
    {}
};

struct PointLight : public Light
{
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    PointLight(
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            glm::vec3 position,
            float constant,
            float linear,
            float quadratic)
            : position(position), constant(constant), linear(linear), quadratic(quadratic),
              Light(ambient, diffuse, specular)
    {}
};

struct SpotLight : public PointLight
{
    glm::vec3 direction;
    float inner_cutoff;
    float outer_cutoff;

    SpotLight(
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            glm::vec3 position,
            float constant,
            float linear,
            float quadratic,
            glm::vec3 direction,
            float inner_cutoff,
            float outer_cutoff)
            : direction(direction), inner_cutoff(inner_cutoff), outer_cutoff(outer_cutoff),
              PointLight(ambient, diffuse, specular, position, constant, linear, quadratic)
    {}
};

