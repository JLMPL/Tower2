#include "LightNode.hpp"

LightNode::LightNode(const vec3& color, bool shadow) :
    m_color(color), m_isShadowCasting(shadow)
{
}

void LightNode::setColor(const vec3& color)
{
    m_color = color;
}

void LightNode::setShadowCasting(bool is)
{
    m_isShadowCasting = is;
}

void LightNode::toggle()
{
    m_on = !m_on;
}

void LightNode::turnOn()
{
    m_on = true;
}

void LightNode::turnOff()
{
    m_on = false;
}

const vec3& LightNode::getColor() const
{
    return m_color;
}

bool LightNode::isShadowCasting() const
{
    return m_isShadowCasting;
}

bool LightNode::isOn() const
{
    return m_on;
}

SceneNode::Type LightNode::getType() const
{
    return Type::Light;
}

const vec3& LightNode::getPosition() const
{
    return m_globalPos;
}

void LightNode::calcsUsingGlobal()
{
    m_globalPos = vec3(m_globalTransform[3]);
}
