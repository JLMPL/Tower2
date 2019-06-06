#include "GraphRenderer.hpp"

namespace gfx
{

GraphRenderer g_GraphRenderer;

void GraphRenderer::init()
{
    m_frameShader.loadFromFile("Shaders/Frame.sha");
    m_screenQuad.init();

    m_shadowPass.init();
    m_basePass.init();
}

void GraphRenderer::finalFrame()
{
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_frameShader.bind();
    m_frameShader.setUniformTexture("image", 0, m_basePass.getRender());
    m_screenQuad.render();
    m_frameShader.unbind();
}

void GraphRenderer::render(SceneGraph& graph)
{
    m_shadowPass.execute(graph);
    m_basePass.execute(graph, m_shadowPass.getRender());

    finalFrame();
}

void GraphRenderer::addLine(const vec3& a, const vec3& b, const vec3& c)
{
    m_basePass.addLine(a,b,c);
}

void GraphRenderer::addSphere(const vec3& pos, f32 rad, const vec3& c)
{
    f32 step = M_TAU / 16.f;
    for (f32 i = 0; i < M_TAU - step; i += step)
    {
        vec3 a = pos + vec3(cos(i) * rad, sin(i) * rad, 0);
        vec3 b = pos + vec3(cos(i + step) * rad, sin(i + step) * rad, 0);
        m_basePass.addLine(a,b,c);

        a = pos + vec3(cos(i) * rad, 0, sin(i) * rad);
        b = pos + vec3(cos(i + step) * rad, 0, sin(i + step) * rad);
        m_basePass.addLine(a,b,c);

        a = pos + vec3(0, cos(i) * rad, sin(i) * rad);
        b = pos + vec3(0, cos(i + step) * rad, sin(i + step) * rad);
        m_basePass.addLine(a,b,c);
    }
}

}
