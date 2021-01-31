#pragma once
#include "Core/Math3D.hpp"

class ParticleAffector
{
public:
    ParticleAffector() = default;
    ParticleAffector(const vec3& pos, f32 str);
    ~ParticleAffector() = default;

    void setPosition(const vec3& pos);
    const vec3& getPosition() const;

    void setStrength(f32 str);
    f32 getStrength() const;

    void enable();
    void disable();

    bool isEnabled() const;

private:
    bool m_enabled = false;

    vec3 m_pos;
    f32 m_strength = 1;
};
