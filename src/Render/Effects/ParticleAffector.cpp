#include "ParticleAffector.hpp"

ParticleAffector::ParticleAffector(const vec3& pos, f32 str) :
    m_pos(pos), m_strength(str)
{
}

void ParticleAffector::setPosition(const vec3& pos)
{
    m_pos = pos;
}

const vec3& ParticleAffector::getPosition() const
{
    return m_pos;
}

void ParticleAffector::setStrength(f32 str)
{
    m_strength = str;
}

f32 ParticleAffector::getStrength() const
{
    return m_strength;
}

void ParticleAffector::enable()
{
    m_enabled = true;
}

void ParticleAffector::disable()
{
    m_enabled = false;
}

bool ParticleAffector::isEnabled() const
{
    return m_enabled;
}

