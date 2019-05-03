#include "Chest.hpp"
#include "Interface/Interface.hpp"
#include "Render/MeshManager.hpp"
#include "Render/MaterialManager.hpp"
#include "Interface/Interface.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "SceneGraph/MeshNode.hpp"
#include "Creature.hpp"
#include "Item.hpp"
#include "Level.hpp"

Chest::Chest(u32 id, LevelContext* context) :
    Interactible(id, context)
{
}

void Chest::init()
{
    m_mesh = m_context->sceneGraph->addMeshNode("chest.obj");
    m_context->sceneGraph->getRoot()->attachNode(m_mesh);

    m_staticBody = m_context->physSys->addStaticBox(vec3(0.25,0.35,0.4), vec3(0));

    m_labelName = "Chest";
    m_offset = 1.f;

    m_requiresTransform = true;
    m_interactTransform =
        math::translate(m_pos) * math::rotate(-HALF_PI, math::vecY) * math::translate(vec3(1,0,0));

    m_interaction.setRequirementFunc([&]() -> bool
    {
        return true;
    });

    m_interaction.setFinishFunc([&](Creature* cre) -> void
    {
        // for (u32 i = 0; i < m_eq.getNumItems(); i++)
        {
            // ui::g_Interface.prompt("Received item:", m_eq.getItemAtIndex(i)->m_name);
            // cre->getEquipment()->give(m_eq.getItemAtIndex(i));
        }
    });

    m_interaction.setAnimation("Chest");
    m_interaction.setCreaturePosition(vec3(3,0,0), vec3(-1,0,0));
}

void Chest::update()
{
    m_mesh->setPosition(m_pos);

    m_transform = math::translate(m_pos);
    m_staticBody.setGlobalTransform(m_transform * math::translate(vec3(0,0.35,0)));

    if (m_interaction.isRunning())
        m_interaction.update();
}

void Chest::interact(Creature* other)
{
    if (f32 dist = math::distance(m_pos, other->getPos()); dist < 1.5f)
    {
        m_interaction.begin(other, this);
    }
}

Entity::Type Chest::getType() const
{
    return Type::Chest;
}
