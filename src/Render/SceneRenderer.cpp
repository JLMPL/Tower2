#include "SceneRenderer.hpp"
#include "Geometry/Geometry.hpp"

namespace gfx
{

SceneRenderer g_SceneRenderer;

void SceneRenderer::init()
{
    m_frameShader.loadFromFile("Shaders/Frame.sha");

    m_shadowPass.init();
    m_basePass.init();
}

void SceneRenderer::finalFrame()
{
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_frameShader.bind();
    m_frameShader.setUniformTexture("image", 0, m_basePass.getRender());
    renderScreenQuad();
    m_frameShader.unbind();
}

void SceneRenderer::render(RenderScene& scene)
{
    m_shadowPass.execute(scene);
    m_basePass.execute(scene, m_shadowPass.getRender());

    finalFrame();
}

void SceneRenderer::addLine(const vec3& a, const vec3& b, const vec3& c)
{
    m_basePass.addLine(a,b,c);
}

void SceneRenderer::addSphere(const vec3& pos, f32 rad, const vec3& c)
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
