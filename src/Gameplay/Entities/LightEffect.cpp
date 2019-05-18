#include "LightEffect.hpp"
#include "Core/FrameInfo.hpp"
#include "Gameplay/Level.hpp"
#include "Gameplay/Level.hpp"
#include "SceneGraph/FlareNode.hpp"
#include "SceneGraph/LightNode.hpp"

LightEffect::LightEffect(u32 id, LevelContext* context, u32 owner) :
    Entity(id, context)
{
    m_light = m_context->sceneGraph->addLightNode();

    auto li = m_light->as<LightNode>();
    li->setColor(vec3(0,50,100));

    m_context->sceneGraph->getRoot()->attachNode(m_light);

    m_flare = m_context->sceneGraph->addFlareNode("flare.png");

    auto fl = m_flare->as<FlareNode>();
    // fl->setScale(0.5f);
    fl->setColor(Color(0,0.7,1,1));

    m_context->sceneGraph->getRoot()->attachNode(m_flare);

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
}

Entity::Type LightEffect::getType() const
{
    return Type::LightEffect;
}

