#include "LightEffect.hpp"
#include "Core/FrameInfo.hpp"
#include "Gameplay/Level.hpp"
#include "SceneGraph/LightNode.hpp"
#include "SceneGraph/FlareNode.hpp"

LightEffect::LightEffect(u32 id, LevelContext* context) :
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
}

void LightEffect::update()
{
    m_angle += core::g_FInfo.delta;
    vec3 tmp = math::rotateY(m_posOffset, m_angle);

    m_light->setPosition(m_pos + tmp);
    m_flare->setPosition(m_light->getPosition());
}

Entity::Type LightEffect::getType() const
{
    return Type::LightEffect;
}

