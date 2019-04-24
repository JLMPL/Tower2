#pragma once
#include <Core/Math3D.hpp>
#include <Render/TargetCamera.hpp>
#include "EventSystem/EventListener.hpp"
#include "Render/OrbitCamera.hpp"

class Creature;

class CameraController final : public EventListener
{
public:
    void init(Creature* player);
    void update();

private:
    void onEvent(const GameEvent& event) override final;

private:
    gfx::OrbitCamera  m_orbitCamera;
    gfx::TargetCamera m_targetCamera;

    Creature*         m_player = nullptr;
    Creature*         m_combatTarget = nullptr;

    vec3              m_closeCenterPos = vec3(0);
    vec3              m_farCenterPos = vec3(-0.25,0,0);
    vec3              m_currCenterPos;

    vec3              m_closeEyePos = vec3(0, 0, -2.5f);
    vec3              m_farEyePos = vec3(-0.25, 0.625f, -1.f);
    vec3              m_currEyePos;

    f32               m_lerp = 0.f;

    bool              m_close = true;
};
