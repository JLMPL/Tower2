#include "CameraController.hpp"
#include "Render/SceneRenderer.hpp"
#include "Gameplay/Level.hpp"
#include "Input/Input.hpp"

void CameraController::init(LevelContext* context)
{
    m_context = context;
    m_camera = m_context->camera;

    m_camera->setEye(vec3(3,3,-3));
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

    m_camera->setCenter(vec3(0));
    m_camera->setEye(m_arm);
}

void CameraController::updateCameraPosition()
{
    m_camera->setCenter(m_player->getPos() + vec3(0,2,0));
    m_camera->setEye(m_player->getPos() + vec3(0,2,0) + m_arm * 4);
    m_camera->updateMatrices();
    m_context->renderScene->setView(m_camera->getView());
}
