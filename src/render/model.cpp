#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <filesystem>

#include "model.hpp"

Model::Model(const std::string& file_path)
        : file_path(file_path)
{
    load_model(file_path);
}

Model::~Model()
{
    for(auto texture : textures)
        delete texture;
}

void Model::draw(Shader& shader)
{
    for(auto& mesh: meshes)
        mesh.draw(shader);
}

std::string Model::get_file_path() const
{
    return file_path;
}

void Model::load_model(const std::string& file_path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw std::runtime_error("Failed to load model: " + file_path);

    file_dir = std::filesystem::path(file_path).parent_path().string();

    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
    for(auto i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    for(auto i = 0; i < node->mNumChildren; i++)
        process_node(node->mChildren[i], scene);
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    std::vector<unsigned int> indices;
    vertices.reserve(3 * mesh->mNumFaces);
    std::vector<Texture*> textures;

    for(auto i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 position(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z);

        glm::vec3 normal(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
        );

        glm::vec2 texture_coords;
        if(mesh->mTextureCoords[0])
        {
            texture_coords.x = mesh->mTextureCoords[0][i].x;
            texture_coords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.emplace_back(position, normal, texture_coords);
    }

    for(auto i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(auto j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture*> diffuse_textures = load_textures(material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuse_textures.begin(), diffuse_textures.end());
        std::vector<Texture*> specular_textures = load_textures(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), specular_textures.begin(), specular_textures.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture*> Model::load_textures(aiMaterial* material, aiTextureType texture_type)
{
    unsigned int texture_count = material->GetTextureCount(texture_type);
    std::vector<Texture*> result;
    for(auto i = 0; i < texture_count; i++)
    {
        aiString path;
        material->GetTexture(texture_type, i, &path);
        std::string texture_path = std::filesystem::path(file_dir).append(path.C_Str()).string();

        auto text_it = std::find_if(textures.begin(), textures.end(),
                [=](auto texture) { return texture->get_file_path() == texture_path; });
        if(text_it == textures.end())
        {
            textures.push_back(new Texture(texture_path));
            text_it = --textures.end();
        }

        result.push_back(*text_it);
    }
    return result;
}
