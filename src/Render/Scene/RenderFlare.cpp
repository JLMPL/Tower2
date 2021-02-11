#include "RenderFlare.hpp"

RenderFlare::RenderFlare(TextureHandle tex) :
    m_texture(tex)
{
    constexpr f32 verts[] = {
        0.5,-0.5,
        0.5,0.5,
        -0.5,0.5,
        0.5,-0.5,
        -0.5,0.5,
        -0.5,-0.5
    };

    constexpr f32 coords[] = {
        1,0,
        1,1,
        0,1,
        1,0,
        0,1,
        0,0
    };

    gfx::createVertexArray(m_vao);
    gfx::bindVertexArray(m_vao);

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(f32) * 12, &verts[0], GL_STATIC_DRAW);
    gfx::setVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_tbo.init(GL_ARRAY_BUFFER);
    m_tbo.bind();
    m_tbo.setData(sizeof(f32) * 12, &coords[0], GL_STATIC_DRAW);
    gfx::setVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    gfx::unbindVertexArray(m_vao);
    m_vbo.unbind();
}

void RenderFlare::render()
{
    bindVertexArray(m_vao);
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    unbindVertexArray(m_vao);
}

void RenderFlare::setColor(const Color& color)
{
    m_color = color;
}

void RenderFlare::setScale(f32 factor)
{
    m_scale = factor;
}

void RenderFlare::setPosition(const vec3& pos)
{
    m_pos = pos;
    transform = math::translate(m_pos);
}
