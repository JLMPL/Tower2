#include "LightEffect.hpp"
#include "Core/FrameInfo.hpp"
#include "Gameplay/Level.hpp"
#include "Gameplay/Level.hpp"
#include "Render/Scene/RenderFlare.hpp"
#include "Render/Scene/RenderLight.hpp"

LightEffect::LightEffect(u32 id, LevelContext* context, u32 owner) :
    Entity(id, context)
{
    m_light = m_context->renderScene->addRenderLight();

    auto li = m_light->as<RenderLight>();
    li->setColor(vec3(0,50,100));

    m_flare = m_context->renderScene->addRenderFlare("flare.png");

    // fl->setScale(0.5f);
    m_flare->setColor(Color(0.5,0.75,1,1));

    m_particleGroup = m_context->particleSys->addParticleGroup(32, vec3(0,0,0));
    m_particleGroup->enableAutoSpawn();
    m_particleGroup->setSpawnPeriod(0.2);
    m_particleGroup->setWeightVariance(0.1, 0.5);
    m_particleGroup->setLifetimeVariance(1, 2);

    m_owner = context->level->getEntityByID(owner);
}

void LightEffect::update()
{
    m_angle -= core::g_FInfo.delta;
    vec3 tmp = math::rotateY(m_posOffset, m_angle);

    f32 y = 2.5 + (sin(m_angle * 4) * 0.05);

    // m_pos = m_owner->getPos() + vec3(0,y,0);

    m_pos = math::lerp(m_pos, m_owner->getPos() + vec3(0,y,0), 10.f * core::g_FInfo.delta);

    m_light->setPosition(m_pos + tmp);
    m_flare->setPosition(m_light->getPosition());
    m_particleGroup->setSpawnPosition(m_light->getPosition());
}

Entity::Type LightEffect::getType() const
{
    return Type::LightEffect;
}

