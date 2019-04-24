#include "FadedRect.hpp"

namespace gfx
{

void FadedRect::init(const vec2& size)
{
    setSize(size);
}

void FadedRect::init(u32 x, u32 y)
{
    setSize({f32(x), f32(y)});
}

void FadedRect::updateGeometry()
{
    const f32 verts[] =
    {
        0,      0,
        0,      m_size.y,
        m_size.x, m_size.y,

        0,      0,
        m_size.x, m_size.y,
        m_size.x, 0,

        m_size.x, m_size.y,
        m_size.x + m_fade, 0,
        m_size.x, 0,

        m_size.x, m_size.y,
        m_size.x + m_fade, m_size.y,
        m_size.x + m_fade, 0,

        -m_fade, m_size.y,
        0, m_size.y,
        0,0,

        -m_fade, m_size.y,
        0, 0,
        -m_fade, 0
    };

    const f32 alphas[] =
    {
        1,1,1,
        1,1,1,
        1,0,1,
        1,0,0,
        0,1,1,
        0,1,0
    };

    m_vao.init();
    m_vao.bind();

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(f32) * 2 * 18, verts, GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_cbo.init(GL_ARRAY_BUFFER);
    m_cbo.bind();
    m_cbo.setData(sizeof(f32) * 18, alphas, GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);

    m_vao.unbind();
    m_vbo.unbind();
}

void FadedRect::render() const
{
    m_vao.bind();
    GL(glDrawArrays(GL_TRIANGLES, 0, 18));
    m_vao.unbind();
}

void FadedRect::setSize(const vec2& size)
{
    m_size = size;
    updateGeometry();
}

void FadedRect::setSize(u32 x, u32 y)
{
    setSize({f32(x), f32(y)});
}

vec2 FadedRect::getSize() const
{
    return m_size;
}

void FadedRect::setColor(const Color& color)
{
    m_color = color;
}

const Color& FadedRect::getColor() const
{
    return m_color;
}

void FadedRect::setFade(f32 fade)
{
    m_fade = fade;
    updateGeometry();
}

f32 FadedRect::getFade() const
{
    return m_fade;
}

void FadedRect::setPosition(const vec2& pos)
{
    m_pos = pos;
}

void FadedRect::setPosition(u32 x, u32 y)
{
    m_pos = {f32(x), f32(y)};
}

vec2 FadedRect::getPosition() const
{
    return m_pos;
}

}
