#include "CameraNode.hpp"
#include "Core/Config.hpp"

CameraNode::CameraNode(float fov)
{
    auto display = core::g_Config.getDisplay();

    m_proj = math::perspective(
        fov,
        f32(display.width) / f32(display.height),
        0.01f,
        2000.f
    );

    m_pos = vec3(-0.6,2,-3);
    m_center = vec3(0,1.5,5);

    m_view = math::lookAt(m_pos, m_center, math::vecY);
}

void CameraNode::calcLocalTransform()
{
    m_localTransform = mat4(1.f);
}

void CameraNode::calcsUsingGlobal()
{
    vec3 pos = vec3(m_globalTransform * vec4(m_pos, 1));
    vec3 center = vec3(m_globalTransform * vec4(m_center, 1));

    m_view = math::lookAt(pos, center, math::vecY);
}

const mat4& CameraNode::getProjection() const
{
    return m_proj;
}

const mat4& CameraNode::getView() const
{
    return m_view;
}

const vec3& CameraNode::getPosition() const
{
    return m_pos;
}

SceneNode::Type CameraNode::getType() const
{
    return Type::Camera;
}
