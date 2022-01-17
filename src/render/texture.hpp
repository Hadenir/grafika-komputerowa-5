#pragma once

#include <string>
#include <glad/glad.h>

class Texture
{
public:
    explicit Texture(const std::string& file_path, GLenum texture_unit = GL_TEXTURE0);
    ~Texture();

    [[nodiscard]] size_t get_width() const;
    [[nodiscard]] size_t get_height() const;

    void bind() const;

    static void unbind();

private:
    GLuint texture_id = 0;
    GLenum texture_unit;

    size_t width, height;
};

