#pragma once

#include <glm/glm.hpp>

#include "../render/shader/shader.hpp"
#include "../render/model.hpp"

class RenderObject
{
public:
    explicit RenderObject(
            Model* model,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

    void draw(PhongShader& shader) const;

    void move(glm::vec3 offset);
    void set_position(glm::vec3 position);
    [[nodiscard]] glm::vec3 get_position() const;

    void rotate(glm::vec3 offset);
    void set_rotation(glm::vec3 rotation);
    [[nodiscard]] glm::vec3 get_rotation() const;

    void set_scale(glm::vec3 scale);

    [[nodiscard]] glm::vec3 get_front_dir() const;

    [[nodiscard]] glm::mat4 get_rotation_matrix() const;
    [[nodiscard]] glm::mat4 get_model_matrix() const;

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Model* model;
};

