#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

Camera::Camera(glm::vec3 initial_position, size_t display_width, size_t display_height)
    : position(initial_position), aspect_ratio((float)display_width / (float)display_height)
{
    update_vectors();
}

void Camera::move(Direction direction, float dt)
{
    float dr = movement_speed * dt;
    switch(direction)
    {
        case Direction::Forward: position += front_dir * dr; break;
        case Direction::Backward: position -= front_dir * dr; break;
        case Direction::Right: position += right_dir * dr; break;
        case Direction::Left: position -= right_dir * dr; break;
        case Direction::Up: position += up_dir * dr; break;
        case Direction::Down: position -= up_dir * dr; break;
    }
    update_vectors();
}

void Camera::set_position(glm::vec3 position)
{
    this->position = position;
    update_vectors();
}

void Camera::rotate(float x_offset, float y_offset)
{
    pitch += y_offset * rotation_sensitivity;
    yaw += x_offset * rotation_sensitivity;

    if(pitch > 89.5f)
        pitch = 89.5f;
    else if(pitch < -89.5f)
        pitch = -89.5f;

    update_vectors();
}

void Camera::set_rotation(float pitch, float yaw)
{
    this->pitch = pitch;
    this->yaw = yaw;
    update_vectors();
}

void Camera::zoom(float offset)
{
    fov += offset * scroll_sensitivity;
    if(fov < 1.0f)
        fov = 1.0f;
    else if(fov > 65.0f)
        fov = 65.0f;
}

void Camera::resize_viewport(size_t display_width, size_t display_height)
{
    aspect_ratio = (float)display_width / (float)display_height;
}

glm::mat4 Camera::get_view_matrix(glm::mat4 model_matrix, glm::mat4 rotation_matrix) const
{
    glm::vec3 world_pos = model_matrix * glm::vec4(position, 1.0);
    glm::vec3 world_look_dir = model_matrix * glm::vec4(position + front_dir, 1.0f);
    glm::vec3 world_up_dir = rotation_matrix * glm::vec4(up_dir, 1.0f);
    return glm::lookAt(world_pos, world_look_dir, world_up_dir);
}

glm::mat4 Camera::get_projection_matrix() const
{
    return glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.0f);
}

glm::vec3 Camera::get_position() const
{
    return position;
}

glm::vec3 Camera::get_direction() const
{
    return front_dir;
}

void Camera::update_vectors()
{
    glm::vec3 front(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    );
    front_dir = glm::normalize(front);
    right_dir = glm::normalize(glm::cross(front_dir, WORLD_UP));
    up_dir = glm::normalize(glm::cross(right_dir, front_dir));
}

