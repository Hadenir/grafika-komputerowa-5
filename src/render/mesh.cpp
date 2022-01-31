#include "mesh.hpp"

#include <utility>

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coords)
    : position(position), normal(normal), texture_coords(texture_coords)
{}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, Material&& material)
    : vertices(std::move(vertices)), indices(std::move(indices)), material(std::move(material))
{
    setup_mesh();
}

void Mesh::draw(PhongShader& shader) const
{
    shader.set_material(material);
    for(auto i = 0; i < material.textures.size(); i++)
    {
        material.textures[i]->bind(GL_TEXTURE0 + i);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::setup_mesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
