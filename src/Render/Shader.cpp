#include "Shader.hpp"
#include "Texture.hpp"
#include "Cubemap.hpp"
#include "Debug/Log.hpp"
#include <fstream>

namespace gfx
{

static std::string name;

Shader::~Shader()
{
    if(m_program)
    {
        GL(glDetachShader(m_program, m_vertex));
        GL(glDetachShader(m_program, m_fragment));
        GL(glDeleteShader(m_vertex));
        GL(glDeleteShader(m_fragment));
        GL(glDeleteProgram(m_program));
    }
}

Shader::ShaderSources Shader::loadSource(const std::string& path)
{
    std::ifstream file(path.c_str());

    if(!file.good())
    {
        Log::error("Could not load %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }

    std::string source[2];
    std::string line;
    u32 mode = 0;

    while(!file.eof())
    {
        std::getline(file, line);

        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                mode = 0;
            else
                mode = 1;
        }
        else
            source[mode] += line + "\n";
    }

    file.close();

    ShaderSources sources;
    sources.vertexShader = source[0];
    sources.fragmentShader = source[1];

    return sources;
}

std::string Shader::loadSeparateSource(const std::string& path)
{
    std::ifstream file(path.c_str());
    std::string source;
    std::string line;

    if (!file.good())
    {
        Log::error("Could not load Shader source %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }

    while (!file.eof())
    {
        std::getline(file, line);
        source += line + "\n";
    }

    file.close();

    return source;
}

GLuint Shader::createShader(const std::string& source, GLuint mode)
{
    GL(GLuint id = glCreateShader(mode));

    const char* csource = source.c_str();

    GL(glShaderSource(id, 1, &csource, NULL));
    GL(glCompileShader(id));

    char error[512];
    GL(glGetShaderInfoLog(id, 512, NULL, error));

    if (strlen(error) != 0)
        Log::info("%s Comile Status: \n%s", name.c_str(), error);

    return id;
}

void Shader::loadFromFile(const std::string& path, const std::string& otherPath)
{
    ShaderSources sources;

    if (otherPath.empty())
        sources = loadSource(path);
    else
    {
        sources.vertexShader = loadSeparateSource(path);
        sources.fragmentShader = loadSeparateSource(otherPath);
    }

    if (otherPath.empty())
        name = path + ":vertex";
    else
        name = path;

    m_vertex = createShader(sources.vertexShader, GL_VERTEX_SHADER);

    if (otherPath.empty())
        name = path + ":fragment";
    else
        name = otherPath;

    m_fragment = createShader(sources.fragmentShader, GL_FRAGMENT_SHADER);

    GL(m_program = glCreateProgram());

    GL(glAttachShader(m_program, m_vertex));
    GL(glAttachShader(m_program, m_fragment));

    GL(glLinkProgram(m_program));
    GL(glValidateProgram(m_program));
}

GLint Shader::getUniformLocation(const char* unif) const
{
    GL(GLuint location = glGetUniformLocation(m_program, unif));

/*    if(location == GL_INVALID_INDEX)
        printf("Location could not be found in shader %s\n", unif);*/

    return location;
}

void Shader::setUniformTexture(const std::string& name, u32 index, const Texture& tex) const
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    tex.bind();
    setUniform1i(name.c_str(), index);
}

void Shader::setUniformTexture(const std::string& name, u32 index, const Cubemap& tex) const
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    tex.bind();
    setUniform1i(name.c_str(), index);
}

void Shader::setUniformTexture(const std::string& name, u32 index, GLuint tex) const
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    GL(glBindTexture(GL_TEXTURE_2D, tex));
    setUniform1i(name.c_str(), index);
}

void Shader::setUniformCubemap(const std::string& name, u32 index, GLuint tex) const
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    GL(glBindTexture(GL_TEXTURE_CUBE_MAP, tex));
    setUniform1i(name.c_str(), index);
}

void Shader::setUniform1i(const char* name, i32 val) const
{
    GL(glUniform1i(getUniformLocation(name), val));
}

void Shader::setUniform1f(const char* name, f32 val) const
{
    GL(glUniform1f(getUniformLocation(name), val));
}

void Shader::setUniform2f(const char* name, f32 x, f32 y) const
{
    GL(glUniform2f(getUniformLocation(name), x, y));
}

void Shader::setUniform2f(const char* name, const UvCoord& vec) const
{
    GL(glUniform2f(getUniformLocation(name), vec.x, vec.y));
}

void Shader::setUniform2f(const char* name, const vec2i& vec) const
{
    GL(glUniform2f(getUniformLocation(name), f32(vec.x), f32(vec.y)));
}

void Shader::setUniform3f(const char* name, f32 x, f32 y, f32 z) const
{
    GL(glUniform3f(getUniformLocation(name), x, y, z));
}

void Shader::setUniform3f(const char* name, const vec3& vec) const
{
    GL(glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z));
}

void Shader::setUniformColor4(const char* name, const Color& col) const
{
    GL(glUniform4f(getUniformLocation(name), col.x, col.y, col.z, col.w));
}

void Shader::setUniformMatrix(const char* name, const mat4& mat) const
{
    GL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::bind() const
{
    GL(glUseProgram(m_program));
}

void Shader::unbind() const
{
    GL(glUseProgram(0));
}

}
