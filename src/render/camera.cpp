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

void Camera::zoom(float offset)
{
    fov += offset * scroll_sensitivity;
    if(fov < 1.0f)
        fov = 1.0f;
    else if(fov > 65.0f)
        fov = 65.0f;
}

glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt(position, position + front_dir, up_dir);
}

glm::mat4 Camera::get_projection_matrix() const
{
    return glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.0f);
}

glm::vec3 Camera::get_position() const
{
    return position;
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
