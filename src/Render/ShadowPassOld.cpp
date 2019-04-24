#include "ShadowPassOld.hpp"
#include "RenderableInstance.hpp"
#include "Light.hpp"

namespace gfx
{

void ShadowPass::init(std::vector<RenderableInstance::Ptr>* allRenderables)
{
    m_allRenderables = allRenderables;

    m_pointShadowShader[Shader::Type::Static].loadFromFile(
        "Shaders/Static.vert", "Shaders/PointShadow.frag");

    m_pointShadowShader[Shader::Type::Skinned].loadFromFile(
        "Shaders/Skinned.vert", "Shaders/PointShadow.frag");

    m_pointShadowShader[Shader::Type::StaticWavy].loadFromFile(
        "Shaders/StaticWavy.vert", "Shaders/PointShadow.frag");

    for (u32 i = 0; i < 2; i++)
    {
        m_pointShadow[i].proj = math::perspective(90.0_rad, 1.f, 0.1f, 100.f);
        m_pointShadow[i].fbo.init(Framebuffer::Type::Color, 512, 512, false, GL_R32F, GL_RED, GL_FLOAT);
        m_pointShadow[i].cubemap.init(512, GL_R32F, GL_RED, GL_FLOAT);
    }
}

void ShadowPass::execute(const Light* light[])
{
    for (u32 l = 0; l < 2; l++)
    {
        vec3 eye = light[l]->pos;

        m_pointShadow[l].views[0] = math::lookAt(eye, eye + vec3(1,0,0), -math::vecY);
        m_pointShadow[l].views[1] = math::lookAt(eye, eye + vec3(-1,0,0), -math::vecY);
        m_pointShadow[l].views[2] = math::lookAt(eye, eye + vec3(0,1,0), math::vecZ);
        m_pointShadow[l].views[3] = math::lookAt(eye, eye + vec3(0,-1,0), -math::vecZ);
        m_pointShadow[l].views[4] = math::lookAt(eye, eye + vec3(0,0,1), -math::vecY);
        m_pointShadow[l].views[5] = math::lookAt(eye, eye + vec3(0,0,-1), -math::vecY);

        m_pointShadow[l].fbo.bind();

        for (auto face = 0; face < 6; face++)
        {
            GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_pointShadow[l].cubemap.getId(), 0));
            GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            for (u32 i = 0; i < m_allRenderables->size(); i++)
            {
                const RenderableInstance* renderable = (*m_allRenderables)[i].get();

                if (renderable)
                {
                    if (!renderable->visible || renderable->nonShadowCasting)
                        continue;

                    auto& shader = m_pointShadowShader[renderable->shaderType];

                    shader.bind();
                    shader.setUniformMatrix("uProj", m_pointShadow[l].proj);
                    shader.setUniformMatrix("uView", m_pointShadow[l].views[face]);
                    shader.setUniform1f("uTime", 1.f);
                    shader.setUniformMatrix("uModel", renderable->transform);
                    shader.setUniform3f("uLightPos", eye);

                    if (renderable->matrixPalette)
                    {
                        for (u32 i = 0; i < renderable->numJoints; i++)
                        {
                            char index[10];
                            sprintf(index, "bones[%d]", i);
                            shader.setUniformMatrix(index, renderable->matrixPalette[i]);
                        }
                    }

                    for (u32 i = 0; i < 8; i++)
                        if (renderable->vao[i])
                            renderable->vao[i]->drawElements();
                    // renderable->vao->drawElements();
                    shader.unbind();
                }
            }
        }

        m_pointShadow[l].fbo.unbind();
    }
}

const Cubemap& ShadowPass::getShadowCubemap(u32 index) const
{
    return m_pointShadow[index].cubemap;
}

}
