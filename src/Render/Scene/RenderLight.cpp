#include "RenderLight.hpp"

RenderLight::RenderLight(const vec3& color, bool shadow) :
    m_color(color), m_isShadowCasting(shadow)
{
}

void RenderLight::setColor(const vec3& color)
{
    m_color = color;
}

void RenderLight::setShadowCasting(bool is)
{
    m_isShadowCasting = is;
}

void RenderLight::toggle()
{
    m_on = !m_on;
}

void RenderLight::turnOn()
{
    m_on = true;
}

void RenderLight::turnOff()
{
    m_on = false;
}

const vec3& RenderLight::getColor() const
{
    return m_color;
}

bool RenderLight::isShadowCasting() const
{
    return m_isShadowCasting;
}

bool RenderLight::isOn() const
{
    return m_on;
}

RenderEntity::Type RenderLight::getType() const
{
    return Type::Light;
}

void RenderLight::setPosition(const vec3& pos)
{
    m_pos = pos;
}

const vec3& RenderLight::getPosition() const
{
    return m_pos;
}
