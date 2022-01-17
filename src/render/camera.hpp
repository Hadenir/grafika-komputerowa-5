#pragma once

#include <glm/glm.hpp>

enum class Direction
{
    Forward, Backward,
    Left, Right,
    Up, Down,
};

class Camera
{
public:
    Camera(glm::vec3 initial_position, size_t display_width, size_t display_height);

    void move(Direction direction, float dt);

    void rotate(float x_offset, float y_offset);

    void zoom(float offset);

    [[nodiscard]] glm::mat4 get_view_matrix() const;

    [[nodiscard]] glm::mat4 get_projection_matrix() const;

    [[nodiscard]] glm::vec3 get_position() const;

private:
    const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 position;
    glm::vec3 front_dir;
    glm::vec3 right_dir;
    glm::vec3 up_dir;

    float yaw = -90, pitch = 0;

    float movement_speed = 2.5f;
    float rotation_sensitivity = 0.1f;
    float scroll_sensitivity = 2.5f;
    float fov = 45.0f;
    float aspect_ratio;

    void update_vectors();
};

