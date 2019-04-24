#include "CameraController.hpp"
#include "Core/FrameInfo.hpp"
#include "Creature.hpp"
#include "Debug/DebugMenu.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Interface/Interface.hpp"
#include "Render/OrbitCamera.hpp"

void CameraController::init(Creature* player)
{
    initEventListening(GameEvent::PlayerRelated);
    g_EventSys.addListener(this);

    m_player = player;

    m_currCenterPos = m_closeCenterPos;
    m_currEyePos = m_closeEyePos;

    ui::g_Interface.setCamera(&m_orbitCamera);

    debug::g_Menu["Camera"].bind("Center", &m_closeCenterPos);
    debug::g_Menu["Camera"].bind("Eye", &m_closeEyePos);
}

void CameraController::onEvent(const GameEvent& event)
{
    switch (event.getType())
    {
        case GameEvent::Player_BowDrawn:
        {
            m_close = false;
            m_lerp = 0;
        }
        break;
        case GameEvent::Player_BowReleased:
        {
            m_close = true;
            m_lerp = 0;
        }
        break;
        case GameEvent::Player_GainCombatFocus:
        {
            m_combatTarget = event.creature;
            m_targetCamera.setEye(m_orbitCamera.getPos());
            m_targetCamera.setCenter(m_orbitCamera.getCenter());
            // gfx::g_Renderer3D.setActiveCamera(&m_targetCamera);
        }
        break;
        case GameEvent::Player_LoseCombatFocus:
        {
            m_combatTarget = nullptr;
            m_orbitCamera.setEye(m_targetCamera.getPos());
            m_orbitCamera.setCenter(m_targetCamera.getCenter());
            // gfx::g_Renderer3D.setActiveCamera(&m_orbitCamera);
        }
        break;
    }
}

void CameraController::update()
{
    if (!m_combatTarget)
    {
        if (m_close)
        {
            m_currCenterPos = math::lerp(m_currCenterPos, m_closeCenterPos, 0.1f);
            m_currEyePos = math::lerp(m_currEyePos, m_closeEyePos, 0.1f);
        }
        else
        {
            m_currCenterPos = math::lerp(m_currCenterPos, m_farCenterPos, 0.1f);
            m_currEyePos = math::lerp(m_currEyePos, m_farEyePos, 0.1f);
        }

        m_orbitCamera.setYaw(m_player->getYaw());
        m_orbitCamera.setCenter(m_player->getPos() + vec3(0,2,0));
        m_orbitCamera.setOriginOffset(m_currCenterPos);
        m_orbitCamera.setOffset(m_currEyePos);
        m_orbitCamera.update();
    }
    else
    {
        vec3 toEnemy = math::normalize(m_combatTarget->getPos() - m_player->getPos());
        vec3 side = math::normalize(math::cross(toEnemy, math::vecY));

        vec3 newEye = m_player->getPos() + vec3(0,2,0) + side + (-toEnemy);
        vec3 newCenter = m_combatTarget->getPos() + vec3(0,1,0);

        m_targetCamera.setEye(math::lerp(m_targetCamera.getPos(), newEye, 0.1f));
        m_targetCamera.setCenter(math::lerp(m_targetCamera.getCenter(), newCenter, 0.1f));
        // m_targetCamera.update();
    }
}
