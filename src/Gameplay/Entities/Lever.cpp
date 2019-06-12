#include "Lever.hpp"
#include "Core/FrameInfo.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Gameplay/Level.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "Render/Scene/RenderMesh.hpp"
#include "Render/Scene/RenderScene.hpp"

Lever::Lever(u32 id, LevelContext* context) : Interactible(id, context)
{
    Base::initLabel("Lever", 0.5f);

    m_mesh = m_context->renderScene->addRenderMesh("sord.obj");
}

void Lever::setActivationTarget(u32 entityID)
{
    m_activationTarget = entityID;
}

void Lever::update()
{
    if (m_activated)
    {
        m_lerp += core::g_FInfo.delta;
        m_lerp = math::clamp(m_lerp, 0.f, 1.f);

        if (m_lerp > 0.9 && !m_flag)
        {
            // GameEvent act(GameEvent::EntityRelated, GameEvent::Entity_Activated);
            // act.setReceiver(m_activationTarget);
            // g_EventSys.enqueue(act);

            m_flag = true;
        }
    }

    m_pitch = math::lerp(-f32(HALF_PI/2), -f32(HALF_PI + HALF_PI/2), m_lerp);

    mat4 tr = math::translate(m_pos) * math::rotate(m_pitch, vec3(0,0,1));

    m_mesh->setTransform(tr);
}

void Lever::interact(Creature* other)
{
    m_activated = true;
}

Entity::Type Lever::getType() const
{
    return Entity::Type::Lever;
}

