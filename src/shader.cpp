#include "shader.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vpath, const std::string& fpath)
	: m_v_filepath(vpath), m_f_filepath(fpath), m_renderer_id(0)
{
    std::string vertex_shader = parse_shader(vpath);
    std::string fragment_shader = parse_shader(fpath);
    m_renderer_id = create_shader(vertex_shader, fragment_shader);
}

Shader::~Shader()
{
    glDeleteProgram(m_renderer_id);
}

std::string Shader::parse_shader(const std::string& path)
{
    std::string code;
    std::ifstream shader_file;

    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shader_file.open(path);
        std::stringstream shader_stream;

        shader_stream << shader_file.rdbuf();

        shader_file.close();

        code = shader_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    return code;
}

unsigned int Shader::compile_shader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << '\n';
        std::cout << message << '\n';
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vert = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int frag = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    // debugging ------------- this didnt work
    /*int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << "Failed to link the shader program: " << message << '\n';
        glDeleteProgram(program);
        return 0;
    }*/
    //------------------------

    glValidateProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

int Shader::get_uniform_location(const std::string& name) const
{
    if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
        return m_uniform_location_cache[name];
   
    int location = glGetUniformLocation(m_renderer_id, name.c_str());
    if (location == -1)
        std::cout << "WARNING: uniform '" << name << "' doesn't exist!" << '\n';
    m_uniform_location_cache[name] = location;
    return location;
}

void Shader::Bind() const
{
    glUseProgram(m_renderer_id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
}

void Shader::SetUniform4f(unsigned int id, float v0, float v1, float v2, float v3)
{
    glUniform4f(id, v0, v1, v2, v3);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(get_uniform_location(name), value);
}

void Shader::SetUniform1i(unsigned int id, int value)
{
    glUniform1i(id, value);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniformMat4f(unsigned int id, const glm::mat4& matrix)
{
    glUniformMatrix4fv(id, 1, GL_FALSE, &matrix[0][0]);
}
