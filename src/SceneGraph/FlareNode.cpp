#include "FlareNode.hpp"

FlareNode::FlareNode(const gfx::Texture* tex) :
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

    m_vao.init();
    m_vao.bind();

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(f32) * 12, &verts[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_tbo.init(GL_ARRAY_BUFFER);
    m_tbo.bind();
    m_tbo.setData(sizeof(f32) * 12, &coords[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_vao.unbind();
    m_vbo.unbind();
}

void FlareNode::render() const
{
    m_vao.bind();
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    m_vao.unbind();
}

void FlareNode::setColor(const Color& color)
{
    m_color = color;
}

void FlareNode::setScale(f32 factor)
{
    m_scale = factor;
}

SceneNode::Type FlareNode::getType() const
{
    return Type::Flare;
}

