#include "Pickup.hpp"
#include "Creature.hpp"
#include "ItemManager.hpp"
#include "Level.hpp"

Pickup::Pickup(u32 id, LevelContext* context) : Interactible(id, context)
{
    m_item = g_ItemMgr.getItem("damn_herb");
    m_labelName = m_item->m_name;
}

void Pickup::addRigidBody(const vec3& pos)
{
    m_hasRigidBody = true;
    m_rigidBody = m_context->physSys->addBox(pos, vec3(0), vec3(0.1));
}

void Pickup::update()
{
    if (m_hasRigidBody)
    {
        m_transform = m_rigidBody.getGlobalTransform();
        m_pos = m_transform[3];
    }
    else
        m_transform = math::translate(m_pos);

    vec4 linez[4] =
    {
        vec4(0,0,0,1),
        vec4(1,0,0,0),
        vec4(0,1,0,0),
        vec4(0,0,1,0)
    };

    for (u32 i = 0; i < 4; i++)
        linez[i] = m_rigidBody.getGlobalTransform() * linez[i];

    // gfx::g_Renderer3D.addLine(linez[0], linez[0] + linez[1], vec3(1,0,0));
    // gfx::g_Renderer3D.addLine(linez[0], linez[0] + linez[2], vec3(0,1,0));
    // gfx::g_Renderer3D.addLine(linez[0], linez[0] + linez[3], vec3(0,0,1));
}

void Pickup::interact(Creature* other)
{
    other->getAnimator().setState("Pickup");
    other->getAnimator().clearPendingFunctions();
}

void Pickup::disable()
{
    Base::disable();

    if (m_hasRigidBody)
        m_rigidBody.disable();

    m_context->physSys->removeRigidBody(m_rigidBody);
    m_hasRigidBody = false;

    Base::destroy();
}

phys::RigidBody& Pickup::getRigidBody()
{
    return m_rigidBody;
}

Entity::Type Pickup::getType() const
{
    return Type::Pickup;
}

