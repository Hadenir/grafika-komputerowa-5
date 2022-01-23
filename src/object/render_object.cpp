#include <glm/gtc/matrix_transform.hpp>

#include "render_object.hpp"

RenderObject::RenderObject(Model&& model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : model(std::move(model)), position(position), rotation(rotation), scale(scale)
{}

void RenderObject::draw(Shader& shader) const
{
    glm::mat4 model_matrix = get_model_matrix();
    shader.set_mat4("modelMat", model_matrix);
    model.draw(shader);
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
