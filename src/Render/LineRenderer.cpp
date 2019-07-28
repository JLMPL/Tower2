#include "LineRenderer.hpp"

namespace gfx
{

LineRenderer::~LineRenderer()
{
    destroyVertexArray(m_vao);
}

void LineRenderer::init()
{
    m_shader.loadFromFile("Shaders/DebugLines.sha");
}

void LineRenderer::addLine(const vec3& a, const vec3& b, const vec3& c)
{
    m_posData.push_back({a,b});
    m_colorData.push_back({c,c});

    m_needsUpdate = true;
}

void LineRenderer::prepare()
{
    if (m_needsUpdate)
    {
        createVertexArray(m_vao);
        bindVertexArray(m_vao);

        m_vbo.init(GL_ARRAY_BUFFER);
        m_vbo.bind();
        m_vbo.setData(sizeof(vec3) * 2 * m_posData.size(), &m_posData[0], GL_STATIC_DRAW);
        setVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        m_cbo.init(GL_ARRAY_BUFFER);
        m_cbo.bind();
        m_cbo.setData(sizeof(vec3) * 2 * m_colorData.size(), &m_colorData[0], GL_STATIC_DRAW);
        setVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        unbindVertexArray(m_vao);
        m_vbo.unbind();
    }
}

void LineRenderer::render(const mat4& view, const mat4& proj)
{
    m_shader.bind();
    m_shader.setUniformMatrix("uProj", proj);
    m_shader.setUniformMatrix("uView", view);

    bindVertexArray(m_vao);
    GL(glDrawArrays(GL_LINES, 0, m_posData.size() * 2));
    unbindVertexArray(m_vao);

    m_shader.unbind();
}

void LineRenderer::clear()
{
    m_posData.clear();
    m_colorData.clear();

    m_needsUpdate = false;
}

}
