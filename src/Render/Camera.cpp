#include "Camera.hpp"

namespace gfx
{

void Camera::updateViewMatrix()
{
}

void Camera::setEye(const vec3& eye)
{
    m_eye = eye;
    updateViewMatrix();
}

void Camera::setCenter(const vec3& center)
{
    m_center = center;
    updateViewMatrix();
}

mat4 Camera::getProjection() const
{
    return m_projection;
}

mat4 Camera::getView() const
{
    return m_view;
}

vec3 Camera::getCenter() const
{
    return m_center;
}

vec3 Camera::getPos() const
{
    return m_eye;
}

vec3 Camera::getForward() const
{
    return math::normalize(vec3(m_dir.x, 0.f, m_dir.z));
}

vec3 Camera::getSide() const
{
    return math::normalize(math::cross(m_dir, vec3(0.f,1.f,0.f)));
}

vec3 Camera::getAim() const
{
    return m_dir;
}

}
