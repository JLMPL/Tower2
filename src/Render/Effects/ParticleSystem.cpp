#include "ParticleSystem.hpp"

ParticleGroup* ParticleSystem::addParticleGroup(u32 numParticles, const vec3& gravity)
{
    m_groups.emplace_back(new ParticleGroup(numParticles, gravity));
    return m_groups.back().get();
}

void ParticleSystem::update()
{
    for (auto& group : m_groups)
    {
        group->update();
        group->debugDraw();
    }
}

