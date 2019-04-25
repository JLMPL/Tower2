#include "Chest.hpp"
#include "Interface/Interface.hpp"
#include "Render/MeshManager.hpp"
#include "Render/MaterialManager.hpp"
#include "Interface/Interface.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "SceneGraph/MeshNode.hpp"
#include "Creature.hpp"
#include "Item.hpp"

void Chest::init(u32 id, const std::string& code, SceneGraph* graph)
{
    Base::init(id, code);
    m_sceneGraph = graph;

    auto mesh = gfx::g_MeshMgr.getMesh("chest.obj");

    m_mesh = m_sceneGraph->addMeshNode("chest.obj");

    m_sceneGraph->getRoot()->attachNode(m_mesh);
    // m_mesh = gfx::g_Renderer3D.addStatic(&mesh->entries[0], gfx::g_MatMgr.getMaterial("env_grass"));

    m_staticBody = phys::g_PhysSys.addStaticBox(vec3(0.25,0.35,0.4), vec3(0));

    m_labelName = "Chest";
    m_offset = 1.f;

    m_requiresTransform = true;
    m_interactTransform =
        math::translate(vec3(1,0,0)) * math::rotate(-HALF_PI, math::vecY);

    m_interaction.setRequirementFunc([&]() -> bool
    {
        return true;
    });

    m_interaction.setFinishFunc([&](Creature* cre) -> void
    {
        for (u32 i = 0; i < m_eq.getNumItems(); i++)
        {
            ui::g_Interface.prompt("Received item:", m_eq.getItemAtIndex(i)->m_name);
            cre->getEquipment()->give(m_eq.getItemAtIndex(i));
        }
    });

    m_interaction.setAnimation("Chest");
    m_interaction.setCameraPosition(vec3(0.5,1.5,-1) + vec3(3,0,0), vec3(-1,0.5,0) + vec3(3,0,0));
    m_interaction.setCreaturePosition(vec3(3,0,0), vec3(-1,0,0));
}

void Chest::update()
{
    m_mesh->setPosition(m_pos);

    m_transform = math::translate(m_pos);
    m_staticBody.setGlobalTransform(m_transform * math::translate(vec3(0,0.35,0)));
    // m_mesh.setTransform(m_transform);

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

Equipment& Chest::getEquipment()
{
    return m_eq;
}

Entity::Type Chest::getType() const
{
    return Type::Chest;
}
