#pragma once
#include <Render/Camera.hpp>
#include <Input/Input.hpp>

namespace gfx
{

class OrbitCamera : public Camera
{
public:
    OrbitCamera();

    void update();

    void setCenter(const vec3& center) override final;
    void setOffset(const vec3& offset);
    void setOriginOffset(const vec3& offset);
    void setYaw(f32 yaw);

private:
    void mouseLook();
    void movementLerp();
    void checkCollisions();
    void updateViewMatrix() override final;

private:
    mat4 m_transform;

    f32  m_pitch = 0.f;
    f32  m_yaw = 0.f;

    vec3 m_targetEye;
    vec3 m_targetCenter;

    vec3 m_offset;
    vec3 m_offsete;
};

}
