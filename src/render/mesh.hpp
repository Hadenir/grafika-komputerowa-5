#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader.hpp"
#include "texture.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coords;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coords);
};

class Mesh
{
public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<Texture*>&& textures);

    void draw(Shader& shader) const;

    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;

    GLuint VAO, VBO, EBO;

    void setup_mesh();
};

