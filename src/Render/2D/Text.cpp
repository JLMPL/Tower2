#include "Text.hpp"
#include "Core/Types.hpp"
#include "Render/Renderer2D.hpp"
#include "Font.hpp"
#include <SDL2/SDL_ttf.h>
#include <vector>

namespace gfx
{

void Text::init(Font* font)
{
    m_font = font;
    setColor({1,1,1,1});
}

void Text::updateGeometry()
{
    auto glyphs = m_font->getPage(m_characterSize)->glyphs;
    m_width = 0;

    f32 cursor = 0;
    std::vector<f32> verts;
    std::vector<f32> coords;

    f32 texel = 1.f / m_font->getPage(m_characterSize)->texture->getWidth();

    for (auto i = 0; i < m_string.size(); i++)
    {
        const auto& glyph = glyphs[m_string[i]];

        verts.push_back(cursor + glyph.bearing.x);
        verts.push_back(-glyph.bearing.y + glyph.size.y);

        verts.push_back(cursor + glyph.bearing.x + glyph.size.x);
        verts.push_back(-glyph.bearing.y + glyph.size.y);

        verts.push_back(cursor + glyph.bearing.x + glyph.size.x);
        verts.push_back(-glyph.bearing.y);

        verts.push_back(cursor + glyph.bearing.x);
        verts.push_back(-glyph.bearing.y + glyph.size.y);

        verts.push_back(cursor + glyph.bearing.x + glyph.size.x);
        verts.push_back(-glyph.bearing.y);

        verts.push_back(cursor + glyph.bearing.x);
        verts.push_back(-glyph.bearing.y);

        f32 xpos = glyph.uvPos.x * texel;
        f32 ypos = glyph.uvPos.y * texel;

        f32 sizex = glyph.size.x * texel;
        f32 sizey = glyph.size.y * texel;

        coords.push_back(xpos);
        coords.push_back(ypos + sizey);

        coords.push_back(xpos + sizex);
        coords.push_back(ypos + sizey);

        coords.push_back(xpos + sizex);
        coords.push_back(ypos);

        coords.push_back(xpos);
        coords.push_back(ypos + sizey);

        coords.push_back(xpos + sizex);
        coords.push_back(ypos);

        coords.push_back(xpos);
        coords.push_back(ypos);

        cursor += glyph.advance;// >> 6;
    }

    m_width = cursor;

    const auto& glyph = glyphs['A'];
    m_height = glyph.size.y;

    m_vao.init();
    m_vao.bind();

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(f32) * verts.size(), &verts[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_tbo.init(GL_ARRAY_BUFFER);
    m_tbo.bind();
    m_tbo.setData(sizeof(f32) * coords.size(), &coords[0], GL_STATIC_DRAW);
    m_vao.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_vao.unbind();
    m_vbo.unbind();
}

void Text::setString(const std::string& str)
{
    m_string = str;
    updateGeometry();
}

void Text::render() const
{
    m_vao.bind();
    GL(glDrawArrays(GL_TRIANGLES, 0, m_string.size() * 12));
    m_vao.unbind();
}

void Text::setCharacterSize(u32 size)
{
    m_characterSize = size;
    updateGeometry();
}

void Text::setColor(const Color& color)
{
    m_color = color;
}

void Text::setPosition(const vec2& vec)
{
    m_pos = vec;
    m_pos.y += m_height;
}

void Text::setPosition(i32 x, i32 y)
{
    m_pos = {f32(x), f32(y)};
    m_pos.y += m_height;
}

vec2 Text::getPosition() const
{
    return m_pos;
}

u32 Text::getWidth() const
{
    return m_width;
}

u32 Text::getHeight() const
{
    return m_height;
}

vec2 Text::getSize() const
{
    return {m_width, m_height};
}

const Texture& Text::getTexture() const
{
    return *m_font->getPage(m_characterSize)->texture;
}

}
