#pragma once
#include "OpenGL.hpp"
#include "Core/Types.hpp"

namespace gfx
{

class VertexArray
{
public:
    VertexArray() = default;
    ~VertexArray();

    void init();

    void setIndexNumber(u32 count);
    void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

    void drawElements() const;

    void bind() const;
    void unbind() const;

private:
    GLuint m_vao = 0;
    u32 m_numIndices = 0;
};

}
