#include "Chest.hpp"
#include "Render/MeshManager.hpp"
#include "Render/MaterialManager.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "SceneGraph/MeshNode.hpp"
#include "Creature.hpp"
#include "Gameplay/Item.hpp"
#include "Gameplay/Level.hpp"

Chest::Chest(u32 id, LevelContext* context) :
    Interactible(id, context)
{
    m_mesh = m_context->sceneGraph->addMeshNode("chest.obj");
    m_context->sceneGraph->getRoot()->attachNode(m_mesh);

    m_staticBody = m_context->physSys->addStaticBox(vec3(0.25,0.35,0.4), vec3(0));

    m_labelName = "Chest";
    m_offset = 1.f;

    m_requiresTransform = true;
    m_interactTransform =
        math::translate(m_pos) * math::rotate(-HALF_PI, math::vecY) * math::translate(vec3(1,0,0));
}

void Chest::update()
{
    m_mesh->setPosition(m_pos);

    m_transform = math::translate(m_pos);
    m_staticBody.setGlobalTransform(m_transform * math::translate(vec3(0,0.35,0)));
}

void Chest::onEvent(const GameEvent& event)
{

}

void Chest::interact(Creature* other)
{
}

Entity::Type Chest::getType() const
{
    return Type::Chest;
}
