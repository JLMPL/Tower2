#include "OrbitCamera.hpp"
#include <Physics/PhysicsSystem.hpp>
#include <Core/Config.hpp>

namespace gfx
{

OrbitCamera::OrbitCamera()
{
    m_offset = vec3(0,1.25f,-3.5f) * 0.5;
    m_offsete = vec3(0,0,0);
    m_targetCenter = vec3(0,2,0);

    auto display = core::g_Config.getDisplay();

    m_projection = math::perspective(
        70.0_rad,
        f32(display.width) / f32(display.height),
        0.01f,
        2000.f
    );
}

void OrbitCamera::mouseLook()
{
    vec2 mouse = gInput.getRightAxis();

    // m_yaw -= f32(mouse.x) / 400.f;
    m_pitch += f32(mouse.y) / 400.f;

    if (m_pitch > HALF_PI - 0.5f)
        m_pitch = HALF_PI - 0.5f;
    if (m_pitch < -HALF_PI + 0.2f)
        m_pitch = -HALF_PI + 0.2f;

    f32 fullRad = M_PI*2;

    if (m_yaw < 0)
        m_yaw += fullRad;
    if (m_yaw > fullRad)
        m_yaw -= fullRad;
}

void OrbitCamera::movementLerp()
{
    m_transform =
        math::translate(m_targetCenter) *
        math::rotate(m_yaw, math::vecY) *
        math::rotate(m_pitch, math::vecX);

    m_eye = vec3(m_transform * vec4(m_offset, 1));
    m_center = vec3(m_transform * vec4(m_offsete, 1));
}

void OrbitCamera::checkCollisions()
{
    vec3 dir = m_eye - m_center;
    f32 farnes = math::length(dir);
    dir = math::normalize(dir);

    auto result = phys::g_PhysSys.sweepSphere(0.05, m_center, dir, farnes);

    if (result.hasHit)
    {
        m_eye = result.pos + result.normal * 0.05;
    }
}

void OrbitCamera::updateViewMatrix()
{
    m_dir = math::normalize(m_center - m_eye);
    m_view = math::lookAt(m_eye, m_center, vec3(0,1,0));
}

void OrbitCamera::update()
{
    mouseLook();
    movementLerp();
    checkCollisions();
    updateViewMatrix();
}

void OrbitCamera::setCenter(const vec3& center)
{
    m_targetCenter = center;
}

void OrbitCamera::setOffset(const vec3& offset)
{
    m_offset = offset;
}

void OrbitCamera::setOriginOffset(const vec3& offset)
{
    m_offsete = offset;
}

void OrbitCamera::setYaw(f32 yaw)
{
    m_yaw = yaw;
}

}
