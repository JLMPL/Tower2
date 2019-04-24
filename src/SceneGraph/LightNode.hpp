#pragma once
#include "SceneNode.hpp"

class LightNode : public SceneNode
{
public:
    LightNode() = default;
    LightNode(const vec3& color, bool shadow = true);
    ~LightNode() = default;

    void setColor(const vec3& color);
    void setShadowCasting(bool is);

    void toggle();
    void turnOn();
    void turnOff();

    const vec3& getColor() const;
    bool isShadowCasting() const;
    bool isOn() const;

    const vec3& getPosition() const override final;

    Type getType() const override final;

private:
    void calcsUsingGlobal() override final;

private:
    vec3 m_color;
    bool m_isShadowCasting = true;
    bool m_on = true;

    vec3 m_globalPos;
};
