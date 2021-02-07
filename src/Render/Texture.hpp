#pragma once
#include "Core/Types.hpp"
#include "Render/OpenGL.hpp"
#include <memory>

namespace gfx
{

class Texture
{
public:
    Texture() = default;
    Texture(const char* path);
    ~Texture();

    void loadFromFile(const std::string& path);
    void create(u32 width, u32 height, void* data);

    u32 getWidth() const;
    u32 getHeight() const;

    void bind() const;
    void unbind() const;

    void reset();

private:
    GLuint  m_texture = 0u;
    u32     m_width = 0u;
    u32     m_height = 0u;
};

}
