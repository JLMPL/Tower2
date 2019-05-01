#include "Door.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Interface/Interface.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "SceneGraph/SceneNode.hpp"
#include "Creature.hpp"

Door::Door(u32 id) : Interactible(id)
{
}

void Door::init(SceneGraph* graph)
{
    initEventListening(GameEvent::EntityRelated);
    g_EventSys.addListener(this);

    // m_renderableHandle = gfx::g_Renderer3D.addStatic(
        // &gfx::g_MeshMgr.getMesh("door.obj")->entries[0], gfx::g_MatMgr.getMaterial("env_wood"), true);

    m_sceneGraph = graph;

    m_mesh = m_sceneGraph->addMeshNode("door.obj");
    m_sceneGraph->getRoot()->attachNode(m_mesh);

    m_staticBody = phys::g_PhysSys.addStaticBox(vec3(0.05,1.5,1), vec3(0,0,0));

    m_labelName = "Door";
    m_offset = 2.f;

    m_requiresTransform = false;

    setDoorState(DoorState::Closed);
}

void Door::setKeyItem(const std::string& key)
{
    m_keyItem = key;
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
            m_lerp += core::g_FInfo.delta;

            if (m_lerp >= 1.f)
            {
                m_lerp = 1.f;
                setDoorState(DoorState::Open);
            }

            m_yaw = math::lerp(0.f, f32(HALF_PI), m_lerp);
        }
        break;

        case DoorState::Open:
            m_yaw = HALF_PI;
            break;
    }

    m_transform =
        math::translate(m_pos) *
        // math::translate(vec3(0,0,0.5)) *
        math::rotate(m_yaw, vec3(0,1,0));

    m_mesh->setPosition(m_pos);
    m_mesh->setRotation(math::rotate(quat(), m_yaw, vec3(0,1,0)));

    mat4 staticBodyTransform =
        math::translate(m_pos) *
        // math::translate(vec3(0,0,0.5)) *
        math::rotate(m_yaw, vec3(0,1,0)) *
        math::translate(vec3(0,1.5,-1));

    m_staticBody.setGlobalTransform(staticBodyTransform);
    // m_renderableHandle.setTransform(m_transform);
}

void Door::interact(Creature* other)
{
    if (f32 dist = math::distance(m_pos, other->getPos());
        dist < 1.5f &&
        m_state == DoorState::Closed/* &&
        other->getEquipment()->hasItem(m_keyItem)*/)
    {
        // other->getEquipment()->take(m_keyItem, 1);
        // setDoorState(DoorState::Opening);
        open();
    }
}

void Door::open()
{
    setDoorState(DoorState::Opening);
}

Entity::Type Door::getType() const
{
    return Type::Door;
}

