#pragma once
#include "Core/Math3D.hpp"
#include "Core/Timer.hpp"

namespace gfx
{

class Camera
{
public:
    Camera() = default;

    virtual void updateViewMatrix();

    void setEye(const vec3& center);
    virtual void setCenter(const vec3& center);

    mat4 getProjection() const;
    mat4 getView() const;
    vec3 getCenter() const;
    vec3 getPos() const;

    vec3 getForward() const;
    vec3 getSide() const;
    vec3 getAim() const;

protected:
    vec3 m_eye;
    vec3 m_center;

    vec3 m_dir = {1,0,0};

    mat4 m_projection;
    mat4 m_view;
};

}
