#include "Camera.hpp"
#include "Core/Config.hpp"
#include "Render/SceneRenderer.hpp"

void Camera::updateMatrices()
{
    auto display = core::getDisplayConfig();

    m_proj = math::perspective(
        m_fov,
        f32(display.width) / f32(display.height),
        0.01f,
        2000.f
    );

    m_view = math::lookAt(m_eye, m_center, math::vecY);

    vec3 dir = m_center - m_eye;
    gfx::g_SceneRenderer.addLine(vec3(0,0,0), vec3(dir.x, 0, dir.z), vec3(1,0,0));
}

void Camera::setFov(f32 fov)
{
    m_fov = fov;
}

void Camera::setEye(const vec3& eye)
{
    m_eye = eye;
}

void Camera::setCenter(const vec3& center)
{
    m_center = center;
}

mat4 Camera::getProjection() const
{
    return m_proj;
}

mat4 Camera::getView() const
{
    return m_view;
}

vec3 Camera::getForward() const
{
    vec3 dir = m_center - m_eye;
    return math::normalize(vec3(dir.x, 0, dir.z));
}

vec3 Camera::getSide() const
{
    return m_side;
}

