#pragma once
#include "Particle.hpp"
#include "ParticleAffector.hpp"
#include <memory>

class ParticleAffector;

class ParticleGroup
{
public:
    using Ptr = std::unique_ptr<ParticleGroup>;

    ParticleGroup() = default;
    ParticleGroup(u32 numParticles, const vec3& gravity);
    ~ParticleGroup();

    void spawnParticle(const Particle& part);
    ParticleAffector* addParticleAffector();

    void update();
    void debugDraw();

    void enableAutoSpawn();
    void disableAutoSpawn();

    void setSpawnPosition(const vec3& pos);
    void setSpawnPeriod(f32 period);
    void setSpawnNumber(u32 number);

    void setPositionVariance(const vec3& min, const vec3& max);
    void setDirectionVariance(const vec3& min, const vec3& max);
    void setVelocityVariance(f32 min, f32 max);
    void setWeightVariance(f32 min, f32 max);
    void setLifetimeVariance(f32 min, f32 max);

private:
    Particle* m_particles = nullptr;
    u32 m_numParticles = 0;

    ParticleAffector m_particleAffector;
    bool m_isAffector = false;

    vec3 m_gravity = vec3(0,-9.8,0);

    bool m_autoSpawn = false;
    f32 m_period = 1;
    u32 m_number = 1;
    vec3 m_origin;
    f32 m_timer = 0;

    vec3 m_posVarMin = vec3(-0.0001);
    vec3 m_posVarMax = vec3(0.0001);

    vec3 m_dirVarMin = vec3(-0.0001);
    vec3 m_dirVarMax = vec3(0.0001);

    vec2 m_velVar;
    vec2 m_weightVar;
    vec2 m_lifeVar;
};
