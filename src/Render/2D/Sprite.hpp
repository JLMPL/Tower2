#pragma once
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"
#include "Core/Math3D.hpp"

namespace gfx
{

class Texture;

struct Rect
{
    f32 x = 0, y = 0;
    f32 w = 32, h = 32;
};

class Sprite
{
public:
    Sprite() = default;

    void init(const Texture* texture, const Rect& dest = {0,0,0,0}, const Rect& src = {0,0,0,0});
    void render() const;

    void setTexture(const Texture* tex);
    void setDest(const Rect& dest);
    void setSource(const Rect& src);
    void setColor(const Color& color);
    void setSize(const vec2& size);

    const Rect& getDest() const;

    const Color& getColor() const;

    void setPosition(const vec2& pos);
    vec2 getPosition() const;

private:
    void update();

private:
    VertexArray m_vao;
    ArrayBuffer m_vbo;
    ArrayBuffer m_tbo;

    const Texture* m_texture = nullptr;

    vec2 m_pos;
    Rect m_dest;
    Rect m_src;
    Color m_color = Color(1);

    friend class Renderer2D;
};

}
