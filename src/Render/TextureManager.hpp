#include "Texture.hpp"
#include <map>

namespace gfx
{

class TextureManager
{
public:
    TextureManager() = default;
    ~TextureManager() = default;

    Texture* getTexture(const std::string& name);

private:
    std::map<std::string, std::unique_ptr<Texture>> m_textures;
};

extern TextureManager g_TexMgr;

};
