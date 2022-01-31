#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader/phong_shader.hpp"
#include "material.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coords;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coords);
};

class Mesh
{
public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, Material&& material);

    void draw(PhongShader& shader) const;

    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;

    GLuint VAO = 0, VBO = 0, EBO = 0;

    void setup_mesh();
};

