#include "Sprite.hpp"
#include "../Texture.hpp"

namespace gfx
{

void Sprite::init(const Texture* texture, const Rect& dest, const Rect& src)
{
    m_texture = texture;
    m_dest = dest;
    m_src = src;

    m_pos = {m_dest.x, m_dest.y};

    if (m_dest.w == 0)
    {
        m_dest.w = m_texture->getWidth();
        m_dest.h = m_texture->getHeight();
    }
    if (m_src.w == 0)
    {
        m_src.w = m_texture->getWidth();
        m_src.h = m_texture->getHeight();
    }

    update();
}

void Sprite::update()
{
    const f32 verts[] =
    {
        0,        0,
        0,        m_dest.h,
        m_dest.w, m_dest.h,

        0,        0,
        m_dest.w, m_dest.h,
        m_dest.w, 0
    };

    f32 texel = 1.f / m_texture->getWidth();
    f32 teyel = 1.f / m_texture->getHeight();

    f32 we = m_src.w * texel;
    f32 he = m_src.h * teyel;

    f32 px = m_src.x * texel;
    f32 py = m_src.y * teyel;

    const f32 coords[] =
    {
        px,      py,
        px,      py + he,
        px + we, py + he,

        px,      py,
        px + we, py + he,
        px + we, py
    };

    createVertexArray(m_vao);
    bindVertexArray(m_vao);

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(f32) * 2 * 6, verts, GL_STATIC_DRAW);
    setVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_tbo.init(GL_ARRAY_BUFFER);
    m_tbo.bind();
    m_tbo.setData(sizeof(f32) * 2 * 6, coords, GL_STATIC_DRAW);
    setVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    unbindVertexArray(m_vao);
    m_vbo.unbind();
}

void Sprite::render()
{
    bindVertexArray(m_vao);
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    unbindVertexArray(m_vao);
}

void Sprite::setTexture(const Texture* tex)
{
    m_texture = tex;
}

void Sprite::setDest(const Rect& dest)
{
    m_dest = dest;
    update();
}

void Sprite::setSource(const Rect& src)
{
    m_src = src;
    update();
}

void Sprite::setColor(const Color& color)
{
    m_color = color;
}

void Sprite::setSize(const vec2& size)
{
    m_dest.w = size.x;
    m_dest.h = size.y;
    update();
}

const Rect& Sprite::getDest() const
{
    return m_dest;
}

const Color& Sprite::getColor() const
{
    return m_color;
}

void Sprite::setPosition(const vec2& pos)
{
    m_pos = pos;
}

vec2 Sprite::getPosition() const
{
    return m_pos;
}

}
