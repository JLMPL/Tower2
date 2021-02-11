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
    class ShadowPass;
};

namespace phys
{
    class Cloth;
}

class ParticleGroup;

class RenderScene
{
public:
    RenderScene();
    ~RenderScene() = default;

    template <typename T, typename... TArgs>
    std::shared_ptr<T> add(TArgs... args)
    {
        if constexpr (std::is_same<T, RenderMesh>::value)
        {
            m_meshes.push_back(std::make_shared<T>(args...));
            return m_meshes.back();
        }
        else if constexpr (std::is_same<T, RenderSkinnedMesh>::value)
        {
            m_skinMeshes.push_back(std::make_shared<T>(args...));
            return m_skinMeshes.back();
        }
        else if constexpr (std::is_same<T, RenderLight>::value)
        {
            m_lights.push_back(std::make_shared<T>(args...));
            return m_lights.back();
        }
        else if constexpr (std::is_same<T, RenderFlare>::value)
        {
            m_flares.push_back(std::make_shared<T>(args...));
            return m_flares.back();
        }
    }

    RenderMesh* addRenderMesh(MeshHandle mesh);
    RenderSkinnedMesh* addRenderSkinnedMesh(MeshHandle mesh, const std::vector<mat4>& palette);
    RenderLight* addRenderLight();
    RenderFlare* addRenderFlare(const std::string& tex);
    // RenderCloth* addRenderCloth(phys::Cloth* cloth);
    // RenderParticles* addRenderParticles(const ParticleGroup* group);

    void setProjection(const mat4& proj);
    void setView(const mat4& view);

    mat4 getProjection() const;
    mat4 getView() const;
    vec3 getCameraPos() const;

private:
    std::vector<RenderMesh::Ref> m_meshes;
    std::vector<RenderSkinnedMesh::Ref> m_skinMeshes;
    std::vector<RenderLight::Ref> m_lights;
    std::vector<RenderFlare::Ref> m_flares;
    // std::vector<RenderCloth::Ptr> m_cloths;
    // std::vector<RenderParticles::Ptr> m_particles;

    mat4 m_proj;
    mat4 m_view;
    vec3 m_pos;

    friend class gfx::BasePass;
    friend class gfx::ShadowPass;
};
