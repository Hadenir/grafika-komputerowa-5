#include <glm/gtc/matrix_transform.hpp>

#include "render_object.hpp"

RenderObject::RenderObject(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        : model(model), position(position), rotation(rotation), scale(scale)
{}

void RenderObject::draw(PhongShader& shader) const
{
    glm::mat4 model_matrix = get_model_matrix();
    shader.set_mat4("modelMat", model_matrix);
    model->draw(shader);
}

glm::mat4 RenderObject::get_model_matrix() const
{
    glm::mat4 model_matrix(1.0f);
    model_matrix = glm::scale(model_matrix, scale);
    model_matrix = glm::rotate(model_matrix, rotation.x * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, rotation.y * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, rotation.z * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    model_matrix = glm::translate(model_matrix, position);
    return model_matrix;
}

void RenderObject::move(glm::vec3 offset)
{
    position += offset;
}

void RenderObject::set_position(glm::vec3 position)
{
    this->position = position;
}

glm::vec3 RenderObject::get_position() const
{
    return position;
}

void RenderObject::rotate(glm::vec3 offset)
{
    rotation += offset;
}

void RenderObject::set_rotation(glm::vec3 rotation)
{
    this->rotation = rotation;
}

glm::vec3 RenderObject::get_rotation() const
{
    return rotation;
}

void RenderObject::set_scale(glm::vec3 scale)
{
    this->scale = scale;
}
