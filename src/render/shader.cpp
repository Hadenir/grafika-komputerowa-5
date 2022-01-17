#include <stdexcept>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

Shader::Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
{
    GLuint vertex_shader = create_shader(vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = create_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

    shader_program_id = create_shader_program(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
    if(shader_program_id != 0)
        glDeleteProgram(shader_program_id);
}

void Shader::use() const
{
    glUseProgram(shader_program_id);
}

void Shader::set_bool(const std::string& name, bool value)
{
    GLint location = glGetUniformLocation(shader_program_id, name.c_str());
    glUniform1i(location, (GLint)value);
}

void Shader::set_int(const std::string& name, int value)
{
    GLint location = glGetUniformLocation(shader_program_id, name.c_str());
    glUniform1i(location, (GLint)value);
}

void Shader::set_float(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(shader_program_id, name.c_str());
    glUniform1f(location, (GLfloat)value);
}

void Shader::set_mat4(const std::string& name, glm::mat4 value)
{
    GLint location = glGetUniformLocation(shader_program_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

GLuint Shader::create_shader(const std::string& shader_source, GLenum shader_type)
{
    const char* vertex_source_str = shader_source.c_str();
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &vertex_source_str, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char info[512];
        glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
        std::stringstream ss;
        ss << "Failed to compile shader: " << info << "!";
        throw std::runtime_error(ss.str());
    }

    return shader;
}

GLuint Shader::create_shader_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success)
    {
        char info[512];
        glGetProgramInfoLog(shader_program, sizeof(info), nullptr, info);
        std::stringstream ss;
        ss << "Failed to link shader program: " << info << "!";
        throw std::runtime_error(ss.str());
    }

    return shader_program;
}
