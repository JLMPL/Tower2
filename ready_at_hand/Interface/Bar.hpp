#pragma once
#include <Render/2D/Rectangle.hpp>

class Renderer2D;

namespace ui
{

class Bar
{
public:
    void init(
        const vec2u& size,
        const Color& color,
        u32 maxValue = 100,
        bool leftie = false
    );
    void draw();

    void setMaxValue(u32 value);
    u32 getMaxValue();

    void setValue(i32 value);
    u32 getValue();

    void setPosition(const vec2& pos);

private:
    void update();

private:
    gfx::Rectangle m_rect;
    gfx::Rectangle m_back;

    bool           m_leftie = false;

    u32            m_width = 0;
    u32            m_height = 0;

    u32            m_maxValue = 1;
    u32            m_value = 1;
};

}
