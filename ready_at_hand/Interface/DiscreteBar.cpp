#include "DiscreteBar.hpp"
#include <Core/FrameInfo.hpp>
#include <Render/Renderer2D.hpp>

static constexpr i32 SpriteSize = 24;
static constexpr i32 HalfSize = SpriteSize / 2;

void DiscreteBar::init(const gfx::Texture* tex, const Color& color, u32 maxValue)
{
    m_color = color;

    m_back.init(tex, {0, 0, SpriteSize * 3, SpriteSize}, {0, 0, SpriteSize * 3, SpriteSize});
    m_back.setColor({0,0,0,0.5});

    m_front.init(tex, {0, 0, SpriteSize * 3, SpriteSize}, {0, 0, SpriteSize * 3, SpriteSize});
    m_front.setColor(m_color);

    m_loss.init(tex, {0, 0, SpriteSize, SpriteSize}, {0, 0, SpriteSize, SpriteSize});
    m_loss.setColor({1,1,0,1});

    setMaxValue(maxValue);
    setPosition({0,0});
}

void DiscreteBar::update()
{
    if (m_prevValue == m_value)
        return;

    m_back.setSource({0, 0, m_maxValue * HalfSize, SpriteSize});
    m_back.setDest({0, 0, m_maxValue * HalfSize, SpriteSize});

    if (m_prevValue > m_value)
    {
        m_added = false;
        m_animFinished = false;
        m_lerp = 0.f;

        m_front.setSource({0, 0, m_value * HalfSize, SpriteSize});
        m_front.setDest({0, 0, m_value * HalfSize, SpriteSize});

        i32 diff = m_prevValue - m_value;

        m_loss.setSource({m_value * HalfSize, 0, diff * HalfSize, SpriteSize});
        m_loss.setDest({0, 0, diff * HalfSize, SpriteSize});
        m_loss.setPosition({m_front.getPosition().x + m_value * HalfSize, m_front.getPosition().y});
    }
    else
    {
        m_added = true;
        m_animFinished = false;
        m_lerp = 0.f;

        i32 diff = m_value - m_prevValue;

        m_loss.setSource({m_prevValue * HalfSize, 0, diff * HalfSize, SpriteSize});
        m_loss.setDest({0, 0, diff * HalfSize, SpriteSize});
        m_loss.setPosition({
            m_front.getPosition().x + (m_prevValue * HalfSize) - HalfSize,
            m_front.getPosition().y
        });
    }
}

void DiscreteBar::draw()
{
    m_lerp += core::g_FInfo.delta * 5;

    gfx::g_Renderer2D.draw(m_back);
    gfx::g_Renderer2D.draw(m_front);

    if (m_prevValue == m_value && m_animFinished)
        return;

    if (m_lerp > 1.f)
    {
        m_animFinished = true;
        m_lerp = 1.f;

        if (m_added)
        {
            m_front.setSource({0, 0, m_value * 12, 24});
            m_front.setDest({0, 0, m_value * 12, 24});
        }
    }

    f32 lurp = m_lerp;
    f32 offset = 0;

    if (m_added)
    {
        lurp = 1.f - m_lerp;
        offset = -12.f;
    }

    m_loss.setColor(math::lerp(m_color, vec4(m_color.x, m_color.y, m_color.z, 0.f), lurp));

    vec2 pos = {m_front.getPosition().x + (m_value * 12) + offset, m_front.getPosition().y};
    m_loss.setPosition(math::lerp(pos, pos + vec2(0,20), lurp));

    gfx::g_Renderer2D.draw(m_loss);
}

void DiscreteBar::setMaxValue(u32 value)
{
    m_maxValue = value;
    update();
}

u32 DiscreteBar::getMaxValue()
{
    return m_maxValue;
}

void DiscreteBar::setValue(i32 value)
{
    m_prevValue = m_value;
    m_value = value;
    update();
}

u32 DiscreteBar::getValue()
{
    return m_value;
}

void DiscreteBar::setPosition(const vec2& pos)
{
    m_pos = pos;

    m_back.setPosition(m_pos + vec2(2));
    m_front.setPosition(m_pos);
}
