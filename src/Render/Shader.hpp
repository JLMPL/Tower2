#pragma once
#include "Core/Types.hpp"
#include "Core/Math3D.hpp"
#include "Render/OpenGL.hpp"
#include <string>
#include <memory>

namespace gfx
{

class Texture;
class Cubemap;

class Shader
{
public:
    using Ptr = std::unique_ptr<Shader>;

    enum Type
    {
        Static,
        StaticWavy,
        Skinned,

        Invalid,
        Count = Invalid
    };

    ~Shader();

    void loadFromFile(const Path& path, const Path& otherPath = std::string());

    GLint getUniformLocation(const char* unif) const;

    void setUniformTexture(const std::string& name, u32 index, const Texture& tex) const;
    void setUniformTexture(const std::string& name, u32 index, const Cubemap& tex) const;
    void setUniformTexture(const std::string& name, u32 index, GLuint tex) const;
    void setUniformCubemap(const std::string& name, u32 index, GLuint tex) const;

    void setUniform1i(const char* name, i32 val) const;
    void setUniform1f(const char* name, f32 val) const;

    void setUniform2f(const char* name, f32 x, f32 y) const;
    void setUniform2f(const char* name, const UvCoord& vec) const;
    void setUniform2f(const char* name, const vec2i& vec) const;

    void setUniform3f(const char* name, f32 x, f32 y, f32 z) const;
    void setUniform3f(const char* name, const vec3& vec) const;

    void setUniformColor4(const char* name, const Color& col) const;
    void setUniformMatrix(const char* name, const mat4& mat) const;

    void bind() const;
    void unbind() const;

private:
    struct ShaderSources
    {
        std::string vertexShader;
        std::string fragmentShader;
    };

    ShaderSources loadSource(const Path& path);
    std::string loadSeparateSource(const Path& path);

    GLuint createShader(const std::string& source, GLuint mode);

private:
    GLuint    m_program = 0;
    GLuint    m_vertex = 0;
    GLuint    m_fragment = 0;
};

}
