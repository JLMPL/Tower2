#pragma once
#include "ThirdParty/entt/entt.hpp"
#include "Render/Material.hpp"

using namespace entt::literals;

struct MaterialLoader : entt::resource_loader<MaterialLoader, gfx::Material>
{
    std::shared_ptr<gfx::Material> load(const std::string& filename) const
    {
        return std::make_shared<gfx::Material>(filename);
    };
};

using _MaterialCache = entt::resource_cache<gfx::Material>;
using MaterialHandle = entt::resource_handle<gfx::Material>;

extern _MaterialCache MaterialCache;