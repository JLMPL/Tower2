#include "Pickup.hpp"
#include "Creature.hpp"
#include "Gameplay/ItemManager.hpp"
#include "Gameplay/Level.hpp"
#include "Render/Scene/RenderMesh.hpp"

Pickup::Pickup(u32 id, LevelContext* context) : Interactible(id, context)
{
    m_item = g_ItemMgr.getItem(0);
    m_labelName = m_item->m_name;
    addRigidBody();

    m_mesh = m_context->renderScene->addRenderMesh("key.obj");
}

void Pickup::addRigidBody(const vec3& pos)
{
    m_hasRigidBody = true;
    m_rigidBody = m_context->physSys->addBox(pos, vec3(0), vec3(0.1,0.2,0.04));
}

void Pickup::update()
{
    if (m_hasRigidBody)
    {
        m_transform = m_rigidBody.getGlobalTransform();
        m_pos = m_transform[3];

        m_mesh->setTransform(m_transform);

        // m_mesh->setPosition(m_transform[3]);
        // m_mesh->setRotation(math::quat_cast(m_transform));
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
    anim::setAnimatorState(&other->getAnimator(), "Pickup");

    destroy();
}

void Pickup::destroy()
{
    m_context->renderScene->removeRenderMesh(m_mesh);
    m_context->physSys->removeRigidBody(m_rigidBody);

    m_destroyed = true;
}

phys::RigidBody& Pickup::getRigidBody()
{
    return m_rigidBody;
}

Entity::Type Pickup::getType() const
{
    return Type::Pickup;
}

