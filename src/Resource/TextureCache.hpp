#pragma once
#include "ThirdParty/entt/entt.hpp"
#include "Render/Texture.hpp"

using namespace entt::literals;

struct TextureLoader : entt::resource_loader<TextureLoader, gfx::Texture>
{
    std::shared_ptr<gfx::Texture> load(const std::string& filename) const
    {
        auto tex = std::make_shared<gfx::Texture>();

        tex->loadFromFile("Textures/" + filename);

        return tex;
    };
};

using _TextureCache = entt::resource_cache<gfx::Texture>;
using TextureHandle = entt::resource_handle<gfx::Texture>;

extern _TextureCache TextureCache;