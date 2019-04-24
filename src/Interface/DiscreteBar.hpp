#pragma once
#include <Render/2D/Sprite.hpp>

namespace gfx
{
    class Texture;
}

class DiscreteBar
{
public:
    void init(
        const gfx::Texture* tex,
        const Color& color,
        u32 maxValue = 6
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
    gfx::Sprite m_back;
    gfx::Sprite m_front;
    gfx::Sprite m_loss;

    Color m_color;

    f32 m_lerp = 0.f;
    bool m_added = true;
    bool m_animFinished = true;

    u32 m_maxValue = 6;
    i32 m_prevValue = 0;
    i32 m_value = 3;

    vec2 m_pos;
};
