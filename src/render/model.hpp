#pragma once

#include <vector>
#include <memory>
#include <assimp/scene.h>

#include "mesh.hpp"
#include "shader.hpp"

class Model
{
public:
    explicit Model(const std::string& file_path);

    void draw(Shader& shader) const;

    [[nodiscard]] std::string get_file_path() const;

    Model(Model&&) = default;
    Model& operator=(Model&&) = default;

private:
    std::vector<Mesh> meshes;
    std::vector<std::unique_ptr<Texture>> textures;
    std::string file_path;
    std::string file_dir;

    void load_model(const std::string& file_path);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> load_textures(aiMaterial* material, aiTextureType texture_type);
};
