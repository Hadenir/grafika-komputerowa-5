#pragma once

#include <string>
#include <glad/glad.h>

class Texture
{
public:
    explicit Texture(const std::string& file_path);
    ~Texture();

    [[nodiscard]] size_t get_width() const;
    [[nodiscard]] size_t get_height() const;

    void bind(GLenum texture_unit = GL_TEXTURE0) const;

    static void unbind();

    [[nodiscard]] std::string get_file_path() const;

    Texture(Texture&&) = default;
    Texture& operator=(Texture&&) = default;

    Texture(const Texture&) = delete;
    void operator=(const Texture&) = delete;

private:
    GLuint texture_id = 0;

    std::string file_path;
    size_t width, height;
};

