#pragma once
#include "Core/Math3D.hpp"

struct LevelContext;

class Entity;
class Camera;

class CameraController
{
public:
    CameraController() = default;
    ~CameraController() = default;

    void init(LevelContext* context);
    void updateCameraRotation();
    void updateCameraPosition();

private:
    LevelContext* m_context = nullptr;
    Camera* m_camera = nullptr;

    Entity* m_player = nullptr;

    f32 m_yaw = 0;
    f32 m_pitch = 0;

    vec3 m_arm;
};
