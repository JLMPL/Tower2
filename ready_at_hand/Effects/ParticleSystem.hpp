#pragma once
#include "ParticleGroup.hpp"
#include <vector>

class ParticleSystem
{
public:
    ParticleSystem() = default;
    ~ParticleSystem() = default;

    ParticleGroup* addParticleGroup(u32 numParticles, const vec3& gravity);

    void update();

private:
    std::vector<ParticleGroup::Ptr> m_groups;
};
