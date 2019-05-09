#include "Font.hpp"
#include "Debug/Log.hpp"
#include <fstream>

namespace gfx
{

Font::Font()
{
    auto error = FT_Init_FreeType(&m_library);
    if (error)
    {
        Log::error("Freetype Initialization Error: %d\n", error);
    }
}

Font::~Font()
{
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);
}

void Font::loadFromFile(const std::string& path)
{
    auto error = FT_New_Face(m_library, path.c_str(), 0, &m_face);

    if (error == FT_Err_Unknown_File_Format)
    {
        Log::error("Could not load font %s! Unknown file format.\n", path.c_str());
    }
    else if (error)
    {
        Log::error("Could not load font %s (%d)\n", path.c_str(), error);
    }
}

void Font::createPage(u32 size)
{
    FT_Set_Pixel_Sizes(m_face, 0, size);

    Page page;
    i32 dualSize = size * 2;
    i32 bufferSize = 16 * dualSize;
    byte* buffer = new byte[bufferSize * bufferSize];
    std::memset(buffer, 0, bufferSize * bufferSize);

    i32 xIndex = 0;
    i32 yIndex = 0;

    for (int i = 0; i < 255; i++)
    {
        int glyphIndex = FT_Get_Char_Index(m_face, i);

        FT_GlyphSlot glyph = m_face->glyph;

        FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_DEFAULT);
        FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);

        int w = glyph->bitmap.width;
        int h = glyph->bitmap.rows;

        page.glyphs[i].uvPos = {xIndex, yIndex};
        page.glyphs[i].size = {w, h};
        page.glyphs[i].bearing = {glyph->bitmap_left, glyph->bitmap_top};
        page.glyphs[i].advance = glyph->advance.x;

        for (auto i = 0; i < h; i++)
        {
            for (auto j = 0; j < w; j++)
            {
                int val = m_face->glyph->bitmap.buffer[j + w * i];
                buffer[(xIndex + j) + bufferSize * (yIndex + i)] = val;
            }
        }

        xIndex += dualSize;

        if (xIndex == bufferSize)
        {
            xIndex = 0;
            yIndex += dualSize;
        }
    }

    page.texture = std::unique_ptr<Texture>(new Texture);
    page.texture->create(bufferSize, bufferSize, buffer);
    free(buffer);

    m_pages[size] = std::move(page);
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
