#include "Bar.hpp"
#include <Render/Renderer2D.hpp>

namespace ui
{

void Bar::init(const vec2u& size, const Color& color, u32 maxValue, bool leftie)
{
    m_leftie = leftie;

    m_width = size.x;
    m_height = size.y;

    m_maxValue = maxValue;

    m_rect.init(size);
    m_rect.setColor(color);

    m_back.init(size);
    m_back.setColor({0,0,0,0.75});

    update();
}

void Bar::update()
{
    f32 step = f32(m_width) / f32(m_maxValue);
    f32 w = f32(m_value) * step;
    m_rect.setSize({w, f32(m_height)});

    if (m_leftie)
        m_rect.setPosition({m_back.getPosition().x + (m_width - w), m_back.getPosition().y});
}

void Bar::draw()
{
    gfx::g_Renderer2D.draw(m_back);
    gfx::g_Renderer2D.draw(m_rect);
}

void Bar::setMaxValue(u32 value)
{
    m_maxValue = math::max(value, 1u);

    update();
}

u32 Bar::getMaxValue()
{
    return m_maxValue;
}

void Bar::setValue(i32 value)
{
    m_value = std::max(0, value);

    update();
}

u32 Bar::getValue()
{
    return m_value;
}

void Bar::setPosition(const vec2& pos)
{
    m_rect.setPosition(pos);
    m_back.setPosition(pos);
}

}
