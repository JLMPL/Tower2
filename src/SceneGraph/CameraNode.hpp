#include "SceneNode.hpp"
#include "Core/Math3D.hpp"

class CameraNode : public SceneNode
{
public:
    CameraNode(float fov = 70.0_rad);
    ~CameraNode() = default;

    const mat4& getProjection() const;
    const mat4& getView() const;

    const vec3& getPosition() const override final;

    Type getType() const override final;

private:
    void calcLocalTransform() override final;
    void calcsUsingGlobal() override final;

private:
    mat4 m_proj;
    mat4 m_view;

    vec3 m_pos;
    vec3 m_center;
};
