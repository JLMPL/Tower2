#pragma once
#include "Core/Math3D.hpp"
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"

namespace gfx
{

class Rectangle
{
public:
    Rectangle() = default;

    void init(const vec2& size);
    void render() const;

    void setSize(const vec2& size);
    vec2 getSize() const;

    void setColor(const Color& color);
    Color getColor() const;

    void setPosition(const vec2& pos);
    void setPosition(u32 x, u32 y);
    vec2 getPosition() const;

private:
    void initBuffers();

private:
    vec2 m_size;
    vec2 m_pos;
    Color m_color;

    VertexArray m_vao;
    ArrayBuffer m_vbo;

    friend class Renderer2D;
};

}
