#include "RenderScene.hpp"
#include "Core/Config.hpp"
#include "Gameplay/Level.hpp"
#include "Render/MeshManager.hpp"
#include "Render/TextureManager.hpp"

RenderScene::RenderScene()
{
    auto display = core::g_Config.getDisplay();

    m_proj = math::perspective(
        70.f,
        f32(display.width) / f32(display.height),
        0.01f,
        2000.f
    );

    m_pos = vec3(3,3,3);
    vec3 center = vec3(0,1,0);

    m_view = math::lookAt(m_pos, center, math::vecY);
}

RenderMesh* RenderScene::addRenderMesh(const std::string& mesh)
{
    m_meshes.emplace_back(new RenderMesh(gfx::g_MeshMgr.getMesh(mesh)));
    return m_meshes.back().get();
}

RenderSkinnedMesh* RenderScene::addRenderSkinnedMesh(const std::string& mesh, const mat4* palette)
{
    m_skinMeshes.emplace_back(new RenderSkinnedMesh(gfx::g_MeshMgr.getSkinnedMesh(mesh), palette));
    return m_skinMeshes.back().get();
}

RenderLight* RenderScene::addRenderLight()
{
    m_lights.emplace_back(new RenderLight(vec3(50)));
    return m_lights.back().get();
}

RenderFlare* RenderScene::addRenderFlare(const std::string& tex)
{
    m_flares.emplace_back(new RenderFlare(gfx::g_TexMgr.getTexture(tex)));
    return m_flares.back().get();
}

RenderCloth* RenderScene::addRenderCloth(phys::Cloth* cloth)
{
    m_cloths.emplace_back(new RenderCloth(cloth));
    return m_cloths.back().get();
}

void RenderScene::setProjection(const mat4& proj)
{
    m_proj = proj;
}

void RenderScene::setView(const mat4& view)
{
    m_view = view;
}

mat4 RenderScene::getProjection() const
{
    return m_proj;
}

mat4 RenderScene::getView() const
{
    return m_view;
}

vec3 RenderScene::getCameraPos() const
{
    return m_pos;
}