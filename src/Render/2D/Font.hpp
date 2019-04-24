#pragma once
#include "Core/Math3D.hpp"
#include "Render/Texture.hpp"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <memory>
#include <map>

namespace gfx
{

struct Glyph
{
    vec2 uvPos;
    vec2 size;
    vec2 bearing;
    i32 advance = 0;
};

struct Page
{
    std::unique_ptr<Texture> texture;
    Glyph glyphs[256];
};

class Font
{
public:
    using Ptr = std::shared_ptr<Font>;

    Font();
    ~Font();

    void loadFromFile(const Path& path);
    Page* getPage(u32 size);

private:
    void createPage(u32 size);

private:
    FT_Library m_library = nullptr;
    FT_Face m_face = nullptr;

    std::map<u32, Page> m_pages;
};

}
