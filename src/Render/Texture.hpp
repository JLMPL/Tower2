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
    Texture(CString path);
    ~Texture();

    void loadFromFile(CString path);
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
