#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
public:
    Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);

    void use() const;

    void set_bool(const std::string& name, bool value);
    void set_int(const std::string& name, int value);
    void set_float(const std::string& name, float value);

private:
    GLuint shader_program_id;

    static GLuint create_shader(const std::string& shader_source, GLenum shader_type);
    static GLuint create_shader_program(GLuint vertex_shader, GLuint fragment_shader);
};

