#include "ShadowPass.hpp"
#include "Geometry/StaticMesh.hpp"
#include "Geometry/SkinnedMesh.hpp"
#include "SceneGraph/CameraNode.hpp"
#include "SceneGraph/LightNode.hpp"
#include "SceneGraph/MeshNode.hpp"
#include "SceneGraph/SkinnedMeshNode.hpp"

namespace gfx
{

void ShadowPass::init()
{
    m_proj = math::perspective(90.0_rad, 1.f, 0.01f, 20.f);
    m_fbo.init(Framebuffer::Type::Color, 256, 256, false, GL_R32F, GL_RED, GL_FLOAT);
    m_cubemap.init(256, GL_R32F, GL_RED, GL_FLOAT);

    m_shadowShader.loadFromFile("Shaders/Static.vert", "Shaders/PointShadow.frag");
    m_ashadowShader.loadFromFile("Shaders/Skinned.vert", "Shaders/PointShadow.frag");
}

void ShadowPass::extractNodes(SceneGraph& graph)
{
    for (auto& i : graph.getNodes())
    {
        switch (i->getType())
        {
            case SceneNode::Type::Camera:
                m_camera = (CameraNode*)(i.get());
                break;
        }
    }

    for (auto& i : graph.getNodes())
    {
        switch (i->getType())
        {
            case SceneNode::Type::Light:
            {
                auto light = (LightNode*)(i.get());

                if (!m_light)
                {
                    m_light = light;
                    continue;
                }

                if (math::length2(m_light->getPosition() - m_camera->getPosition()) <
                    math::length2(light->getPosition() - m_camera->getPosition()))
                {
                    m_light = light;
                }
            }
        }
    }
}

void ShadowPass::execute(SceneGraph& graph)
{
    if (!graph.getRoot()->hasChildren())
        return;

    extractNodes(graph);

    vec3 eye = m_light->getPosition();

    m_views[0] = math::lookAt(eye, eye + vec3(1,0,0), -math::vecY);
    m_views[1] = math::lookAt(eye, eye + vec3(-1,0,0), -math::vecY);
    m_views[2] = math::lookAt(eye, eye + vec3(0,1,0), math::vecZ);
    m_views[3] = math::lookAt(eye, eye + vec3(0,-1,0), -math::vecZ);
    m_views[4] = math::lookAt(eye, eye + vec3(0,0,1), -math::vecY);
    m_views[5] = math::lookAt(eye, eye + vec3(0,0,-1), -math::vecY);

    m_fbo.bind();

    for (auto face = 0; face < 6; face++)
    {
        GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_cubemap.getId(), 0));
        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (auto& node : graph.getNodes())
        {
            if (!node->isVisible())
                continue;

            if (node->getType() == SceneNode::Type::Mesh)
            {
                auto meshNode = (MeshNode*)(node.get());
                auto mesh = meshNode->getMesh();

                for (auto& entry : mesh->entries)
                {
                    auto vao = &entry.vao;

                    m_shadowShader.bind();
                    m_shadowShader.setUniformMatrix("uProj", m_proj);
                    m_shadowShader.setUniformMatrix("uView", m_views[face]);
                    m_shadowShader.setUniformMatrix("uModel", meshNode->getGlobalTransform());
                    m_shadowShader.setUniform3f("uLightPos", eye);

                    vao->drawElements();

                    m_shadowShader.unbind();
                }
            }
            else if (node->getType() == SceneNode::Type::SkinMesh)
            {
                auto meshNode = (SkinnedMeshNode*)(node.get());
                auto mesh = meshNode->getMesh();

                for (auto& entry : mesh->entries)
                {
                    auto vao = &entry.vao;

                    m_ashadowShader.bind();
                    m_ashadowShader.setUniformMatrix("uProj", m_proj);
                    m_ashadowShader.setUniformMatrix("uView", m_views[face]);
                    m_ashadowShader.setUniformMatrix("uModel", meshNode->getGlobalTransform());
                    m_ashadowShader.setUniform3f("uLightPos", eye);

                    for (u32 j = 0; j < meshNode->getNumJoints(); j++)
                    {
                        char index[10];
                        sprintf(index, "bones[%d]", j);
                        m_ashadowShader.setUniformMatrix(index, meshNode->getMatrixPalette()[j]);
                    }

                    vao->drawElements();

                    m_ashadowShader.unbind();
                }
            }
        }
    }

    m_fbo.unbind();
}

GLuint ShadowPass::getRender() const
{
    return m_cubemap.getId();
}

}
