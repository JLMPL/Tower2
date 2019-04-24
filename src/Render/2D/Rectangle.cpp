#include "Rectangle.hpp"

namespace gfx
{

void Rectangle::init(const vec2& size)
{
    setSize(size);

    const f32 verts[] =
    {
        0,0,
        0,1,
        1,1,

        0,0,
        1,1,
        1,0
    };

    m_vao.init();
    m_vao.bind();

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(f32) * 2 * 6, verts, GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_vao.unbind();
    m_vbo.unbind();
}

void Rectangle::render() const
{
    m_vao.bind();
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    m_vao.unbind();
}

void Rectangle::setSize(const vec2& size)
{
    m_size = size;
}

vec2 Rectangle::getSize() const
{
    return m_size;
}

void Rectangle::setColor(const Color& color)
{
    m_color = color;
}

Color Rectangle::getColor() const
{
    return m_color;
}

void Rectangle::setPosition(const vec2& pos)
{
    m_pos = pos;
}

void Rectangle::setPosition(u32 x, u32 y)
{
    m_pos = {f32(x), f32(y)};
}

vec2 Rectangle::getPosition() const
{
    return m_pos;
}

}
