#pragma once
#include "Core/Math3D.hpp"
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"

namespace gfx
{

class FadedRect
{
public:
    FadedRect() = default;

    void init(const vec2& size);
    void init(u32 x, u32 y);

    void render();

    void setSize(const vec2& size);
    void setSize(u32 x, u32 y);
    vec2 getSize() const;

    void setColor(const Color& color);
    const Color& getColor() const;

    void setFade(f32 fade);
    f32 getFade() const;

    void setPosition(const vec2& pos);
    void setPosition(u32 x, u32 y);
    vec2 getPosition() const;

private:
    void updateGeometry();

private:
    vec2 m_size;
    vec2 m_pos;
    Color m_color = {0,0,0,1};
    f32 m_fade = 32.f;

    VertexArray m_vao;
    ArrayBuffer m_vbo;
    ArrayBuffer m_cbo;
};

}
