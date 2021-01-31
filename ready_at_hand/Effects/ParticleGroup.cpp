#include "ParticleGroup.hpp"
#include "Core/FrameInfo.hpp"
#include "Render/SceneRenderer.hpp"
#include "Core/Random.hpp"

ParticleGroup::ParticleGroup(u32 numParticles, const vec3& gravity) :
    m_numParticles(numParticles), m_gravity(gravity)
{
    m_particles = (Particle*)malloc(sizeof(Particle) * m_numParticles);

    for (auto i = 0; i < m_numParticles; i++)
        m_particles[i] = Particle();
}

ParticleGroup::~ParticleGroup()
{
    free(m_particles);
}

void ParticleGroup::spawnParticle(const Particle& part)
{
    for (auto i = 0; i < m_numParticles; i++)
    {
        if (!m_particles[i].isUsed)
        {
            m_particles[i] = part;
            m_particles[i].isUsed = true;
            break;
        }
    }
}

ParticleAffector* ParticleGroup::addParticleAffector()
{
    m_isAffector = true;
    return &m_particleAffector;
}

void ParticleGroup::update()
{
    m_timer += core::g_FInfo.delta;

    if (m_autoSpawn)
    {
        if (m_timer >= m_period)
        {
            m_timer = 0;

            Particle p;
            for (u32 i = 0; i < m_number; i++)
            {
                p.pos = m_origin + vec3(
                    core::rand::inRange(m_posVarMin[0], m_posVarMax[0]),
                    core::rand::inRange(m_posVarMin[1], m_posVarMax[1]),
                    core::rand::inRange(m_posVarMin[2], m_posVarMax[2])
                );

                p.vel = math::normalize(vec3(
                    core::rand::inRange(m_dirVarMin[0], m_dirVarMax[0]),
                    core::rand::inRange(m_dirVarMin[1], m_dirVarMax[1]),
                    core::rand::inRange(m_dirVarMin[2], m_dirVarMax[2])
                )) * core::rand::inRange(m_velVar[0], m_velVar[1]);

                p.weight = core::rand::inRange(m_weightVar[0], m_weightVar[1]);
                p.lifetime = core::rand::inRange(m_lifeVar[0], m_lifeVar[1]);

                spawnParticle(p);
            }
        }
    }

    for (auto i = 0; i < m_numParticles; i++)
    {
        if (m_particles[i].isUsed)
        {
            auto& p = m_particles[i];
            const f32& delta = core::g_FInfo.delta;

            if (p.timer >= p.lifetime)
                m_particles[i].isUsed = false;

            p.vel += m_gravity * p.weight * delta;

            if (m_isAffector)
            {
                vec3 affectorPos = m_particleAffector.getPosition();
                vec3 diff = affectorPos - p.pos;

                f32 strength = m_particleAffector.getStrength() / (math::length(diff) * math::length(diff));

                vec3 dir = diff * strength;
                p.vel += dir * delta;
            }

            p.pos += p.vel * delta;
            p.timer += delta;
        }
    }
}

void ParticleGroup::debugDraw()
{
    for (auto i = 0; i < m_numParticles; i++)
    {
        if (m_particles[i].isUsed)
        {
            auto& p = m_particles[i];

            gfx::g_SceneRenderer.addLine(p.pos, p.pos + vec3(0,0.05,0), vec3(0,1,0));
        }
    }
}

void ParticleGroup::enableAutoSpawn()
{
    m_autoSpawn = true;
}

void ParticleGroup::disableAutoSpawn()
{
    m_autoSpawn = false;
}

void ParticleGroup::setSpawnPosition(const vec3& pos)
{
    m_origin = pos;
}

void ParticleGroup::setSpawnPeriod(f32 period)
{
    m_period = period;
}

void ParticleGroup::setSpawnNumber(u32 number)
{
    m_number = number;
}

void ParticleGroup::setPositionVariance(const vec3& min, const vec3& max)
{
    m_posVarMin = min;
    m_posVarMax = max;
}

void ParticleGroup::setDirectionVariance(const vec3& min, const vec3& max)
{
    m_dirVarMin = min;
    m_dirVarMax = max;
}

void ParticleGroup::setVelocityVariance(f32 min, f32 max)
{
    m_velVar = vec2(min, max);
}

void ParticleGroup::setWeightVariance(f32 min, f32 max)
{
    m_weightVar = vec2(min, max);
}

void ParticleGroup::setLifetimeVariance(f32 min, f32 max)
{
    m_lifeVar = vec2(min, max);
}
