#pragma once

#include <vector>
#include <assimp/scene.h>

#include "mesh.hpp"
#include "shader.hpp"

class Model
{
public:
    explicit Model(const std::string& file_path);
    ~Model();

    void draw(Shader& shader);

    [[nodiscard]] std::string get_file_path() const;

private:
    std::vector<Mesh> meshes;
    std::vector<Texture*> textures;
    std::string file_path;
    std::string file_dir;

    void load_model(const std::string& file_path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> load_textures(aiMaterial* material, aiTextureType texture_type);
};
