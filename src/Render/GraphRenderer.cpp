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

}
