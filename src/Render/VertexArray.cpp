#include "VertexArray.hpp"

namespace gfx
{

void createVertexArray(VertexArray& vao)
{
    GL(glDeleteVertexArrays(1, &vao.id));
    GL(glGenVertexArrays(1, &vao.id));
}

void destroyVertexArray(VertexArray& vao)
{
    GL(glDeleteVertexArrays(1, &vao.id));
}

void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
    GL(glEnableVertexAttribArray(index));
    GL(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
}

void drawVertexArray(VertexArray& vao)
{
    GL(glBindVertexArray(vao.id));
    GL(glDrawElements(GL_TRIANGLES, vao.numIndices, GL_UNSIGNED_INT, 0));
    GL(glBindVertexArray(0));
}

void bindVertexArray(VertexArray& vao)
{
    GL(glBindVertexArray(vao.id));
}

void unbindVertexArray(VertexArray& vao)
{
    GL(glBindVertexArray(0));
}

}
