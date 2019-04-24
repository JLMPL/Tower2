#include "FontManager.hpp"

namespace gfx
{

FontManager g_FontMgr;

Font* FontManager::getFont(const String& name)
{
    auto found = m_fonts.find(name);

    if (found != m_fonts.end())
        return (*found).second.get();

    std::unique_ptr<Font> font(new Font());
    font->loadFromFile(String("Fonts/" + name).c_str());
    m_fonts[name] = std::move(font);

    return m_fonts[name].get();
}

}
