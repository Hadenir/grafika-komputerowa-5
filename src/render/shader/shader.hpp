#pragma once

#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"

class Shader
{
public:
    Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
    virtual ~Shader();

    void use() const;

    void set_bool(const std::string& name, bool value);
    void set_int(const std::string& name, int value);
    void set_float(const std::string& name, float value);
    void set_vec3(const std::string& name, glm::vec3 value);
    void set_vec4(const std::string& name, glm::vec4 value);
    void set_mat4(const std::string& name, glm::mat4 value);

    Shader(const Shader&) = delete;
    void operator=(const Shader&) = delete;

private:
    GLuint shader_program_id = 0;

    GLint get_uniform_location(const std::string& name) const;

    static GLuint create_shader(const std::string& shader_source, GLenum shader_type);
    static GLuint create_shader_program(GLuint vertex_shader, GLuint fragment_shader);
};

