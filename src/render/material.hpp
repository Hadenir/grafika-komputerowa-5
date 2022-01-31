#pragma once

#include <vector>
#include "texture.hpp"

struct Material
{
    std::vector<Texture*> textures;
    float shininess;

    Material(std::vector<Texture*>&& textures, float shininess)
        : textures(std::move(textures)), shininess(shininess)
    {}
};
