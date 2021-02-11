#include "BasePass.hpp"
#include "Render/Scene/RenderMesh.hpp"
#include "Render/Scene/RenderSkinnedMesh.hpp"
#include "Render/Scene/RenderFlare.hpp"
#include "Render/Scene/RenderCloth.hpp"
#include "Geometry/Geometry.hpp"
#include "Material.hpp"
#include "Core/Config.hpp"

namespace gfx
{

void BasePass::init()
{
    m_fbo.init(gfx::Framebuffer::Type::ColorAndDepth, core::getDisplayConfig().width, core::getDisplayConfig().height);

    m_flatShader.loadFromFile("Shaders/Static.vert", "Shaders/Flat.frag");
    m_aflatShader.loadFromFile("Shaders/Skinned.vert", "Shaders/Flat.frag");
    m_clothShader.loadFromFile("Shaders/Static.vert", "Shaders/Cloth.frag");
    m_flareShader.loadFromFile("Shaders/Flare.sha");
    m_particleShader.loadFromFile("Shaders/Particles.sha");

    m_lines.init();
}

void BasePass::extractNodes(RenderScene& scene)
{
    for (auto& light : scene.m_lights)
    {
        m_lights.push_back(light.get());
    }
}

void BasePass::sortLights(RenderScene& scene)
{
    if (m_lights.size() > 1)
    {
        std::sort(m_lights.begin(), m_lights.end(),
        [&](const RenderLight* a, const RenderLight* b)
        {
            return(
                math::length2(a->getPosition() - scene.getCameraPos()) >
                math::length2(b->getPosition() - scene.getCameraPos()));
        });
    }

    while (m_lights.size() < 4)
    {
        m_lights.push_back(&m_nullLight);
    }
}

void BasePass::renderMeshes(RenderScene& scene, GLuint shadow0)
{
    // for (auto& cloth : scene.m_cloths)
    // {
    //     if (cloth->isVisible())
    //         cloth->updateGeometry();
    // }

    // for (auto& part : scene.m_particles)
    // {
    //     if (part->isVisible())
    //         part->updateTransforms();
    // }

    m_fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    for (auto& renderMesh : scene.m_meshes)
    {
        if (!renderMesh->visible)
            continue;

        auto mesh = renderMesh->mesh;

        for (auto& entry : mesh->entries)
        {
            auto vao = &entry.vao;
            auto mat = entry.material;

            m_flatShader.bind();
            m_flatShader.setUniformMatrix("uProj", scene.getProjection());
            m_flatShader.setUniformMatrix("uView", scene.getView());
            m_flatShader.setUniformMatrix("uModel", renderMesh->transform);
            m_flatShader.setUniform3f("uCamPos", scene.getCameraPos());

            for (u32 i = 0; i < 4; i++)
            {
                std::string index = std::to_string(i);

                std::string pos = "uPointLights[" + index + "].pos";
                std::string color = "uPointLights[" + index + "].color";

                m_flatShader.setUniform3f(pos.c_str(), m_lights[i]->getPosition());
                m_flatShader.setUniform3f(color.c_str(), m_lights[i]->getColor());
            }

            m_flatShader.setUniformTexture("uImage", 0, *mat->m_diffuse);
            m_flatShader.setUniformTexture("uNormal", 1, *mat->m_normal);
            m_flatShader.setUniformTexture("uSpecular", 2, *mat->m_specular);

            m_flatShader.setUniformCubemap("uPointShadowCubemap", 3, shadow0);

            // vao->drawElements();
            gfx::drawVertexArray(*vao);

            m_flatShader.unbind();
        }
    }

    // for (auto& renderCloth : scene.m_cloths)
    // {
    //     if (!renderCloth->isVisible())
    //         continue;

    //     auto mat = renderCloth->getMaterial();

    //     m_clothShader.bind();
    //     m_clothShader.setUniformMatrix("uProj", scene.getProjection());
    //     m_clothShader.setUniformMatrix("uView", scene.getView());
    //     m_clothShader.setUniformMatrix("uModel", renderCloth->getTransform());
    //     m_clothShader.setUniform3f("uCamPos", scene.getCameraPos());

    //     for (u32 i = 0; i < 4; i++)
    //     {
    //         std::string index = std::to_string(i);

    //         std::string pos = "uPointLights[" + index + "].pos";
    //         std::string color = "uPointLights[" + index + "].color";

    //         m_clothShader.setUniform3f(pos.c_str(), m_lights[i]->getPosition());
    //         m_clothShader.setUniform3f(color.c_str(), m_lights[i]->getColor());
    //     }

    //     m_clothShader.setUniformTexture("uImage", 0, *mat->m_diffuse);
    //     m_clothShader.setUniformTexture("uNormal", 1, *mat->m_normal);
    //     m_clothShader.setUniformTexture("uSpecular", 2, *mat->m_specular);

    //     m_clothShader.setUniformCubemap("uPointShadowCubemap", 3, shadow0);

    //     drawVertexArray(renderCloth->m_vao);

    //     m_clothShader.unbind();
    // }

    for (auto& renderSkin : scene.m_skinMeshes)
    {
        if (!renderSkin->visible)
            continue;

        auto mesh = renderSkin->mesh;

        for (auto& entry : mesh->entries)
        {
            auto vao = &entry.vao;
            auto mat = entry.material;

            m_aflatShader.bind();
            m_aflatShader.setUniformMatrix("uProj", scene.getProjection());
            m_aflatShader.setUniformMatrix("uView", scene.getView());
            m_aflatShader.setUniformMatrix("uModel", renderSkin->transform);
            m_aflatShader.setUniform3f("uCamPos", scene.getCameraPos());

            for (u32 j = 0; j < renderSkin->matrixPalette.size(); j++)
            {
                char index[10];
                sprintf(index, "bones[%d]", j);
                m_aflatShader.setUniformMatrix(index, renderSkin->matrixPalette[j]);
            }

            for (u32 i = 0; i < 4; i++)
            {
                std::string index = std::to_string(i);

                std::string pos = "uPointLights[" + index + "].pos";
                std::string color = "uPointLights[" + index + "].color";

                m_aflatShader.setUniform3f(pos.c_str(), m_lights[i]->getPosition());
                m_aflatShader.setUniform3f(color.c_str(), m_lights[i]->getColor());
            }

            m_aflatShader.setUniformTexture("uImage", 0, *mat->m_diffuse);
            m_aflatShader.setUniformTexture("uNormal", 1, *mat->m_normal);
            m_aflatShader.setUniformTexture("uSpecular", 2, *mat->m_specular);

            m_aflatShader.setUniformCubemap("uPointShadowCubemap", 3, shadow0);

            gfx::drawVertexArray(*vao);

            m_aflatShader.unbind();
        }
    }

    m_lines.render(scene.getView(), scene.getProjection());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    for (auto& renderFlare : scene.m_flares)
    {
        if (!renderFlare->visible)
            continue;

        m_flareShader.bind();
        m_flareShader.setUniformMatrix("uProj", scene.getProjection());
        m_flareShader.setUniformMatrix("uView", scene.getView());
        m_flareShader.setUniformMatrix("uModel", renderFlare->transform);

        m_flareShader.setUniform1f("uScale", renderFlare->m_scale);
        m_flareShader.setUniformColor4("uColor", renderFlare->m_color);

        m_flareShader.setUniformTexture("uTexture", 0, *renderFlare->m_texture);

        renderFlare->render();

        m_flareShader.unbind();
    }

    // for (auto& renderParticles : scene.m_particles)
    // {
    //     if (!renderParticles->isVisible())
    //         continue;

    //     auto tex = g_TexMgr.getTexture("flare.png");

    //     m_particleShader.bind();
    //     m_particleShader.setUniformMatrix("uProj", scene.getProjection());
    //     m_particleShader.setUniformMatrix("uView", scene.getView());
    //     m_particleShader.setUniformTexture("uTexture", 0, *tex);

    //     for (auto i = 0; i < renderParticles->getParticleCount(); i++)
    //     {
    //         m_particleShader.setUniformMatrix("uModel", renderParticles->getMatrixArray()[i]);
    //         renderParticles->render();
    //     }

    //     m_particleShader.unbind();
    // }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    m_fbo.unbind();
}

void BasePass::execute(RenderScene& scene, GLuint shadow0)
{
    m_lines.prepare();

    extractNodes(scene);
    sortLights(scene);
    renderMeshes(scene, shadow0);

    m_lights.clear();
    m_lines.clear();
}

void BasePass::addLine(const vec3& a, const vec3& b, const vec3& c)
{
    m_lines.addLine(a,b,c);
}

}

