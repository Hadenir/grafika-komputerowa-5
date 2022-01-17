#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

#include "texture.hpp"

Texture::Texture(const std::string& file_path, GLenum texture_unit)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
    if(!data)
        throw std::runtime_error("Failed to load texture file!");

    glActiveTexture(texture_unit);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GL_LINEAR);

    GLenum format;
    switch(channels)
    {
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: throw std::runtime_error("Unsupported image format!");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    this->width = width;
    this->height = height;
    this->texture_unit = texture_unit;
}

Texture::~Texture()
{
    if(texture_id != 0)
        glDeleteTextures(1, &texture_id);
}

size_t Texture::get_width() const
{
    return width;
}

size_t Texture::get_height() const
{
    return height;
}

void Texture::bind() const
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
