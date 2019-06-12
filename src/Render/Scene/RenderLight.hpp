#pragma once
#include "RenderEntity.hpp"

class RenderLight : public RenderEntity
{
public:
    using Ptr = std::unique_ptr<RenderLight>;

    RenderLight() = default;
    RenderLight(const vec3& color, bool shadow = true);
    ~RenderLight() = default;

    void setColor(const vec3& color);
    void setShadowCasting(bool is);

    void toggle();
    void turnOn();
    void turnOff();

    const vec3& getColor() const;
    bool isShadowCasting() const;
    bool isOn() const;

    void setPosition(const vec3& pos);
    const vec3& getPosition() const;

    Type getType() const override final;

private:
    vec3 m_color;
    bool m_isShadowCasting = true;
    bool m_on = true;

    vec3 m_pos;
};
