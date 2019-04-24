#pragma once
#include "Core/Math3D.hpp"
#include "../ArrayBuffer.hpp"
#include "../Texture.hpp"
#include "../VertexArray.hpp"
#include <string>

namespace gfx
{

class Font;

class Text
{
public:
    Text() = default;

    void init(Font* font);
    void setString(const std::string& str);
    void render() const;

    void setCharacterSize(u32 size);
    void setColor(const Color& color);

    void setPosition(const vec2& vec);
    void setPosition(i32 x, i32 y);
    vec2 getPosition() const;

    u32 getWidth() const;
    u32 getHeight() const;
    vec2 getSize() const;

    const Texture& getTexture() const;

private:
    void updateGeometry();

private:
    Font* m_font = nullptr;
    std::string m_string;

    VertexArray m_vao;
    ArrayBuffer m_vbo;
    ArrayBuffer m_tbo;

    f32 m_width = 0;
    f32 m_height = 0;

    vec2 m_pos;
    Color m_color;
    u32 m_characterSize = 12u;

    friend class Renderer2D;
};

}
