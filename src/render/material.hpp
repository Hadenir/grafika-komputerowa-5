#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "texture.hpp"

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    std::vector<Texture*> textures;

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::vector<Texture*>&& textures)
            : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess),
              textures(std::move(textures))
    {}
};
