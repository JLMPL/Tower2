#include "VertexArray.hpp"

namespace gfx
{

VertexArray::~VertexArray()
{
    GL(glDeleteVertexArrays(1, &m_vao));
}

void VertexArray::init()
{
    GL(glDeleteVertexArrays(1, &m_vao));
    GL(glGenVertexArrays(1, &m_vao));
}

void VertexArray::setIndexNumber(u32 count)
{
    m_numIndices = count;
}

void VertexArray::vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
    GL(glEnableVertexAttribArray(index));
    GL(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
}

void VertexArray::drawElements() const
{
    GL(glBindVertexArray(m_vao));
    GL(glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0));
    GL(glBindVertexArray(0));
}

void VertexArray::bind() const
{
    GL(glBindVertexArray(m_vao));
}

void VertexArray::unbind() const
{
    GL(glBindVertexArray(0));
}

}
