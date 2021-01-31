#pragma once
#include "Core/Math3D.hpp"
#include "Render/Texture.hpp"
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

    void loadFromFile(const std::string& path);
    Page* getPage(u32 size);

private:
    void createPage(u32 size);

private:
    byte* m_fontBuffer = nullptr;

    std::map<u32, Page> m_pages;
};

}
