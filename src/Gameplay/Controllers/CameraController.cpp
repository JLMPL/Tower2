#include "CameraController.hpp"
#include "Render/SceneRenderer.hpp"
#include "Gameplay/Level.hpp"
#include "Input/Input.hpp"

void CameraController::init(LevelContext* context)
{
    m_context = context;
    m_camera = m_context->camera;

    m_camera->setEye(vec3(0,3,0));
    m_camera->setCenter(vec3(0,0,0));

    m_player = m_context->level->getEntityByID(0);
}

void CameraController::updateCameraRotation()
{
    vec2 rightAxis = gInput.getRightAxis();

    m_yaw -= rightAxis.x * 0.0025f;
    m_pitch += rightAxis.y * 0.0025f;

    m_pitch = math::clamp(m_pitch, -0.6f, 0.9f);

    m_arm = vec3(0,0,-1);
    m_arm = math::rotateX(m_arm, m_pitch);
    m_arm = math::rotateY(m_arm, m_yaw);

    // m_camera->setCenter(vec3(0));
    m_camera->setCenter(m_player->getPos() + vec3(0,1,0));
    m_camera->setEye(m_arm);

    // m_camera->setEye(vec3(0,3,0));
}

void CameraController::updateCameraPosition()
{
    // vec3 diff = m_camera->getPos() - m_player->getPos();
    // diff.y = 0;
    // f32 dist = math::length(diff);

    // if (dist > 8.f)
    // {
    //     f32 offset = dist - 8.f;
    //     m_camera->setEye(m_camera->getPos() - (math::normalize(diff) * offset));
    // }
    // else if (dist < 5.f)
    // {
    //     f32 offset = 5.f - dist;
    //     m_camera->setEye(m_camera->getPos() + (math::normalize(diff) * offset));
    // }

    // vec3 eye = m_camera->getPos();
    // eye.y = m_player->getPos().y + 4.f;
    // m_camera->setEye(eye);

    vec3 dir = m_camera->getPos() - (m_player->getPos() + vec3(0,1,0));

    auto result = m_context->physSys->sweepSphere(0.5f, (m_player->getPos() + vec3(0,1,0)) + math::normalize(dir) * 1.f, dir, math::length(dir));

    if (result.hasHit)
    {
        m_camera->setEye(result.pos + result.normal * 0.5f);
    }

    m_camera->updateMatrices();
    m_context->renderScene->setView(m_camera->getView());

    m_camera->setCenter(m_player->getPos() + vec3(0,2,0));
    m_camera->setEye(m_player->getPos() + vec3(0,2,0) + m_arm * 4);
    m_camera->updateMatrices();
    m_context->renderScene->setView(m_camera->getView());
}
