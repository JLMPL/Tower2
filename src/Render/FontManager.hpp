#include "Render/2D/Font.hpp"

namespace gfx
{

class FontManager
{
public:
    FontManager() = default;
    ~FontManager() = default;

    Font* getFont(const String& name);

private:
    std::map<String, std::unique_ptr<Font>> m_fonts;
};

extern FontManager g_FontMgr;

}
