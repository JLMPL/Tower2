#include "Door.hpp"
#include "Creature.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Gameplay/Level.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "SceneGraph/SceneNode.hpp"

Door::Door(u32 id, LevelContext* context) : Entity(id, context)
{
    m_mesh = m_context->sceneGraph->addMeshNode("door.obj");
    m_context->sceneGraph->getRoot()->attachNode(m_mesh);

    m_staticBody = m_context->physSys->addStaticBox(vec3(0.05,1.5,1), vec3(0,0,0));

    setDoorState(DoorState::Closed);
    open();
}

void Door::onEvent(const GameEvent& event)
{
    if (event.getReceiver() != m_id)
        return;

    open();
}

void Door::setDoorState(const DoorState& state)
{
    m_state = state;

    if (state != DoorState::Open)
        m_lerp = 0;
}

void Door::update()
{
    switch (m_state)
    {
        case DoorState::Closed:
            m_yaw = 0.f;
            break;

        case DoorState::Opening:
        {
            m_lerp += core::g_FInfo.delta * 0.1;

            if (m_lerp >= 1.f)
            {
                m_lerp = 1.f;
                setDoorState(DoorState::Open);
            }

            // m_yaw = math::lerp(0.f, f32(HALF_PI), m_lerp);
            m_raise = math::lerp(0.f, 3.f, m_lerp);
        }
        break;

        case DoorState::Open:
            // m_yaw = HALF_PI;
            m_raise = 3;
            break;
    }

    m_mesh->setPosition(m_pos + vec3(0, m_raise, 0));
    m_mesh->setRotation(math::rotate(quat(), m_yaw, vec3(0,1,0)));

    mat4 staticBodyTransform =
        math::translate(m_pos + vec3(0, m_raise, 0)) *
        // math::translate(vec3(0,0,0.5)) *
        math::rotate(m_yaw, vec3(0,1,0)) *
        math::translate(vec3(0,1.5,-1));

    m_staticBody.setGlobalTransform(staticBodyTransform);
}

void Door::open()
{
    setDoorState(DoorState::Opening);
}

Entity::Type Door::getType() const
{
    return Type::Door;
}

