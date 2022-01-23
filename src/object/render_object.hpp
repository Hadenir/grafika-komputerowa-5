#pragma once

#include <glm/glm.hpp>

#include "../render/shader.hpp"
#include "../render/model.hpp"

class RenderObject
{
public:
    explicit RenderObject(
            Model&& model,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

    void draw(Shader& shader) const;

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Model model;

    [[nodiscard]] glm::mat4 get_model_matrix() const;
};

