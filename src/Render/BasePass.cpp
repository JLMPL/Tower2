#include "BasePass.hpp"
#include "SceneGraph/CameraNode.hpp"
#include "SceneGraph/MeshNode.hpp"
#include "SceneGraph/SkinnedMeshNode.hpp"
#include "SceneGraph/FlareNode.hpp"
#include "Geometry/StaticMesh.hpp"
#include "Geometry/SkinnedMesh.hpp"
#include "Material.hpp"

namespace gfx
{

void BasePass::init()
{
    m_fbo.init(gfx::Framebuffer::Type::ColorAndDepth, 1920, 1080);

    m_flatShader.loadFromFile("Shaders/Static.vert", "Shaders/Flat.frag");
    m_aflatShader.loadFromFile("Shaders/Skinned.vert", "Shaders/Flat.frag");

    m_flareShader.loadFromFile("Shaders/Flare.sha");

    m_lines.init();
}

void BasePass::extractNodes(SceneGraph& graph)
{
    for (auto& i : graph.getNodes())
    {
        switch (i->getType())
        {
            case SceneNode::Type::Camera:
                m_cameraNode = dynamic_cast<CameraNode*>(i.get());
                m_fuckView = m_cameraNode->getView();
                m_fuckProj = m_cameraNode->getProjection();
                break;
            case SceneNode::Type::Light:
                m_lights.push_back((LightNode*)(i.get()));
                break;
        }
    }
}

void BasePass::sortLights()
{
    if (m_lights.size() > 1)
    {
        std::sort(m_lights.begin(), m_lights.end(),
        [this](const LightNode* a, const LightNode* b)
        {
            return(
                math::length2(a->getPosition() - m_cameraNode->getPosition()) >
                math::length2(b->getPosition() - m_cameraNode->getPosition()));
        });
    }

    while (m_lights.size() < 4)
    {
        m_lights.push_back(&m_nullLight);
    }
}

void BasePass::renderMeshes(SceneGraph& graph, GLuint shadow0)
{
    m_fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    if (graph.getRoot()->hasChildren())
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
                auto mat = entry.material;

                m_flatShader.bind();
                m_flatShader.setUniformMatrix("uProj", m_cameraNode->getProjection());
                m_flatShader.setUniformMatrix("uView", m_cameraNode->getView());
                m_flatShader.setUniformMatrix("uModel", meshNode->getGlobalTransform());
                m_flatShader.setUniform3f("uCamPos", m_cameraNode->getPosition());

                for (u32 i = 0; i < 4; i++)
                {
                    std::string index = std::to_string(i);

                    std::string pos = "uPointLights[" + index + "].pos";
                    std::string color = "uPointLights[" + index + "].color";

                    m_flatShader.setUniform3f(pos.c_str(), m_lights[i]->getPosition());
                    m_flatShader.setUniform3f(color.c_str(), m_lights[i]->getColor());
                }

                m_flatShader.setUniformTexture("uImage", 0, *mat->m_textures[0]);
                m_flatShader.setUniformTexture("uNormal", 1, *mat->m_textures[1]);
                m_flatShader.setUniformTexture("uSpecular", 2, *mat->m_textures[2]);

                m_flatShader.setUniformCubemap("uPointShadowCubemap", 3, shadow0);

                vao->drawElements();

                m_flatShader.unbind();
            }
        }
        else if (node->getType() == SceneNode::Type::SkinMesh)
        {
            auto meshNode = (SkinnedMeshNode*)(node.get());
            auto mesh = meshNode->getMesh();

            for (auto& entry : mesh->entries)
            {
                auto vao = &entry.vao;
                auto mat = entry.material;

                m_aflatShader.bind();
                m_aflatShader.setUniformMatrix("uProj", m_cameraNode->getProjection());
                m_aflatShader.setUniformMatrix("uView", m_cameraNode->getView());
                m_aflatShader.setUniformMatrix("uModel", meshNode->getGlobalTransform());
                m_aflatShader.setUniform3f("uCamPos", m_cameraNode->getPosition());

                for (u32 j = 0; j < meshNode->getNumJoints(); j++)
                {
                    char index[10];
                    sprintf(index, "bones[%d]", j);
                    m_aflatShader.setUniformMatrix(index, meshNode->getMatrixPalette()[j]);
                }

                for (u32 i = 0; i < 4; i++)
                {
                    std::string index = std::to_string(i);

                    std::string pos = "uPointLights[" + index + "].pos";
                    std::string color = "uPointLights[" + index + "].color";

                    m_aflatShader.setUniform3f(pos.c_str(), m_lights[i]->getPosition());
                    m_aflatShader.setUniform3f(color.c_str(), m_lights[i]->getColor());
                }

                m_aflatShader.setUniformTexture("uImage", 0, *mat->m_textures[0]);
                m_aflatShader.setUniformTexture("uNormal", 1, *mat->m_textures[1]);
                m_aflatShader.setUniformTexture("uSpecular", 2, *mat->m_textures[2]);

                m_aflatShader.setUniformCubemap("uPointShadowCubemap", 3, shadow0);

                vao->drawElements();

                m_aflatShader.unbind();
            }
        }
    }

    m_lines.render(m_fuckView, m_fuckProj);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    for (auto& node : graph.getNodes())
    {
        if (!node->isVisible())
            continue;

        if (node->getType() == SceneNode::Type::Flare)
        {
            if (!m_cameraNode)
                printf("fuck2\n");
            auto flare = (FlareNode*)(node.get());

            m_flareShader.bind();
            m_flareShader.setUniformMatrix("uProj", m_cameraNode->getProjection());
            m_flareShader.setUniformMatrix("uView", m_cameraNode->getView());
            m_flareShader.setUniformMatrix("uModel", flare->getGlobalTransform());

            m_flareShader.setUniform1f("uScale", flare->m_scale);
            m_flareShader.setUniformColor4("uColor", flare->m_color);

            m_flareShader.setUniformTexture("uTexture", 0, *flare->m_texture);

            flare->render();

            m_flareShader.unbind();
        }
    }
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    m_fbo.unbind();
}

void BasePass::execute(SceneGraph& graph, GLuint shadow0)
{
    m_lines.prepare();

    extractNodes(graph);
    sortLights();
    renderMeshes(graph, shadow0);

    m_lights.clear();
    m_lines.clear();
}

void BasePass::addLine(const vec3& a, const vec3& b, const vec3& c)
{
    m_lines.addLine(a,b,c);
}

}

