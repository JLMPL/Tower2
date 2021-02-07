#pragma once
#include "ThirdParty/entt/entt.hpp"
#include "Render/Geometry/Geometry.hpp"

using namespace entt::literals;

struct StaticMeshLoader : entt::resource_loader<StaticMeshLoader, gfx::Mesh>
{
    std::shared_ptr<gfx::Mesh> load(const std::string& filename) const
    {
        auto mesh = std::make_shared<gfx::Mesh>();

        loadStaticMeshFromFile(*mesh, "Meshes/" + filename, false);

        return mesh;
    };
};

struct SkinnedMeshLoader : entt::resource_loader<SkinnedMeshLoader, gfx::Mesh>
{
    std::shared_ptr<gfx::Mesh> load(const std::string& filename) const
    {
        auto mesh = std::make_shared<gfx::Mesh>();

        loadSkinnedMeshFromFile(*mesh, "Meshes/" + filename, false);

        return mesh;
    };
};

using _MeshCache = entt::resource_cache<gfx::Mesh>;
using MeshHandle = entt::resource_handle<gfx::Mesh>;

extern _MeshCache MeshCache;