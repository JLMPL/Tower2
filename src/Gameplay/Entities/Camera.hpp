#pragma once
#include "Core/Math3D.hpp"

class Camera
{
public:
    Camera() = default;
    ~Camera() = default;

    void updateMatrices();

    void setFov(f32 fov);
    void setEye(const vec3& eye);
    void setCenter(const vec3& center);

    mat4 getProjection() const;
    mat4 getView() const;

    vec3 getForward() const;
    vec3 getSide() const;

private:
    vec3 m_eye;
    vec3 m_center;

    mat4 m_proj;
    mat4 m_view;

    vec3 m_forward;
    vec3 m_side;

    f32 m_fov = 70.f;
};
