#include "Camera.hpp"
#include "Core/Config.hpp"

void Camera::updateMatrices()
{
    auto display = core::g_Config.getDisplay();

    m_proj = math::perspective(
        m_fov,
        f32(display.width) / f32(display.height),
        0.01f,
        2000.f
    );

    m_view = math::lookAt(m_eye, m_center, math::vecY);
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

