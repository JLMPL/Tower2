#pragma once
#include "OpenGL.hpp"
#include "Core/Types.hpp"

namespace gfx
{

struct VertexArray
{
    GLuint id = 0;
    u32 numIndices = 0;
};

void createVertexArray(VertexArray& vao);
void destroyVertexArray(VertexArray& vao);
void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
void drawVertexArray(VertexArray& vao);
void bindVertexArray(VertexArray& vao);
void unbindVertexArray(VertexArray& vao);

}
