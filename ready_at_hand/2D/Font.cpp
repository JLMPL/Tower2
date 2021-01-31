#include "Font.hpp"
#include "Debug/Log.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#include "ThirdParty/stb/stb_truetype.h"
#include <fstream>

namespace gfx
{

Font::Font()
{
}

Font::~Font()
{
    free(m_fontBuffer);
}

void Font::loadFromFile(const std::string& path)
{
    FILE* file = fopen(path.c_str(), "r");

    if (!file)
    {
        Log::error("Could not load font %s\n", path.c_str());
    }

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    m_fontBuffer = (byte*)malloc(fileSize);
    fread(m_fontBuffer, 1, fileSize, file);
    fclose(file);
}

void Font::createPage(u32 size)
{
    Page page;
    stbtt_pack_context packContext;

    byte* datas = (byte*)malloc(1024*1024);
    stbtt_packedchar* packedChars = (stbtt_packedchar*)malloc(sizeof(stbtt_packedchar) * 256);

    stbtt_PackBegin(&packContext, datas, 1024, 1024, 0, 1, NULL);
    stbtt_PackSetOversampling(&packContext, 1, 1);
    stbtt_PackFontRange(&packContext, m_fontBuffer, 0, size, 0, 256, packedChars);
    stbtt_PackEnd(&packContext);

    for (int i = 0; i < 256; i++)
    {
        const stbtt_packedchar& pc = packedChars[i];

        page.glyphs[i].uvPos = {pc.x0, pc.y0};
        page.glyphs[i].size = {pc.x1 - pc.x0, pc.y1 - pc.y0};
        page.glyphs[i].bearing = {pc.xoff, -pc.yoff};
        page.glyphs[i].advance = pc.xadvance;
    }

    page.texture = std::unique_ptr<Texture>(new Texture);
    page.texture->create(1024, 1024, datas);
    m_pages[size] = std::move(page);

    free(packedChars);
    free(datas);
}

Page* Font::getPage(u32 size)
{
    auto found = m_pages.find(size);

    if (found == m_pages.end())
    {
        createPage(size);
    }

    return &m_pages[size];
}

}
