#include "TextureManager.hpp"

namespace gfx
{

TextureManager g_TexMgr;

Texture* TextureManager::getTexture(const std::string& name)
{
    auto found = m_textures.find(name);

    if (found != m_textures.end())
        return (*found).second.get();

    std::unique_ptr<Texture> texture(new Texture());
    texture->loadFromFile(std::string("Textures/" + name).c_str());
    m_textures[name] = std::move(texture);

    return m_textures[name].get();
}

}
