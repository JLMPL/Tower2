#pragma once
#include "RenderMesh.hpp"
#include "RenderSkinnedMesh.hpp"
#include "RenderLight.hpp"
#include "RenderFlare.hpp"
#include "RenderCloth.hpp"

namespace gfx
{
    class StaticMesh;
    class Mesh;
    class GraphRenderer;
    class ShadowPass;
};

namespace phys
{
    class Cloth;
}

class RenderScene
{
public:
    RenderScene();
    ~RenderScene() = default;

    RenderMesh* addRenderMesh(const std::string& mesh);
    RenderSkinnedMesh* addRenderSkinnedMesh(const std::string& mesh, const mat4* palette);
    RenderLight* addRenderLight();
    RenderFlare* addRenderFlare(const std::string& tex);
    RenderCloth* addRenderCloth(phys::Cloth* cloth);

    void setProjection(const mat4& proj);
    void setView(const mat4& view);

    mat4 getProjection() const;
    mat4 getView() const;
    vec3 getCameraPos() const;

private:
    std::vector<RenderMesh::Ptr> m_meshes;
    std::vector<RenderSkinnedMesh::Ptr> m_skinMeshes;
    std::vector<RenderLight::Ptr> m_lights;
    std::vector<RenderFlare::Ptr> m_flares;
    std::vector<RenderCloth::Ptr> m_cloths;

    mat4 m_proj;
    mat4 m_view;
    vec3 m_pos;

    friend class gfx::GraphRenderer;
    friend class gfx::BasePass;
    friend class gfx::ShadowPass;
};
