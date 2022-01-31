#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <filesystem>
#include <queue>

#include "model.hpp"

Model::Model(std::vector<Mesh>&& meshes, std::vector<std::unique_ptr<Texture>>&& textures)
    : meshes(std::move(meshes)), textures(std::move(textures))
{}

Model::Model(const std::string& file_path)
        : file_path(file_path)
{
    load_model(file_path);
}

void Model::draw(PhongShader& shader) const
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

    std::queue<aiNode*> nodes;
    nodes.push(scene->mRootNode);
    while(!nodes.empty())
    {
        aiNode* node = nodes.front();
        nodes.pop();

        for(auto i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(process_mesh(mesh, scene));
        }

        for(auto i = 0; i < node->mNumChildren; i++)
            nodes.push(node->mChildren[i]);
    }
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

    if(mesh->mMaterialIndex < 0) throw std::runtime_error("Mesh has no material");

    aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture*> diffuse_textures = load_textures(ai_material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuse_textures.begin(), diffuse_textures.end());
    std::vector<Texture*> specular_textures = load_textures(ai_material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specular_textures.begin(), specular_textures.end());

    float shininess;
    ai_material->Get(AI_MATKEY_SHININESS, shininess);
    aiColor3D ambient;
    ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    aiColor3D diffuse;
    ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    aiColor3D specular;
    ai_material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    Material material(
            glm::vec3(ambient.r, ambient.g, ambient.b),
            glm::vec3(diffuse.r, diffuse.g, diffuse.b),
            glm::vec3(specular.r, specular.g, specular.b),
            shininess,
            std::move(textures));

    return Mesh(std::move(vertices), std::move(indices), std::move(material));
}

std::vector<Texture*> Model::load_textures(aiMaterial* material, aiTextureType texture_type)
{
    unsigned int texture_count = material->GetTextureCount(texture_type);
    std::vector<Texture*> result;
    for(auto i = 0; i < texture_count; i++)
    {
        aiString path;
        material->GetTexture(texture_type, i, &path);
        std::string texture_path = file_dir + '/' + path.C_Str();

        auto text_it = std::find_if(textures.begin(), textures.end(),
                [=](auto& texture) { return texture->get_file_path() == texture_path; });
        if(text_it == textures.end())
        {
            textures.push_back(std::make_unique<Texture>(texture_path));
            text_it = --textures.end();
        }

        result.push_back(text_it->get());
    }
    return result;
}
