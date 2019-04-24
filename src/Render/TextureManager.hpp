#include "Texture.hpp"
#include <map>

namespace gfx
{

class TextureManager
{
public:
    TextureManager() = default;
    ~TextureManager() = default;

    Texture* getTexture(const String& name);

private:
    std::map<String, std::unique_ptr<Texture>> m_textures;
};

extern TextureManager g_TexMgr;

};
