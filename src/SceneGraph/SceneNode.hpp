#pragma once
#include "Core/Math3D.hpp"
#include <vector>
#include <memory>

class SceneNode
{
public:
    using Ptr = std::unique_ptr<SceneNode>;

    enum class Type
    {
        Empty,
        Camera,
        Mesh,
        SkinMesh,
        Light
    };

    template <typename T>
    T* as()
    {
        return static_cast<T*>(this);
    }

    SceneNode() = default;
    ~SceneNode() = default;

    void attachNode(SceneNode* node);
    void detachNode(SceneNode* node);

    virtual void onAttach() {}
    virtual void onDetach() {}

    void updateTransforms(mat4 parent);

    void setRotation(const quat& qt);
    const quat& getRotation() const;

    void setPosition(const vec3& pos);
    virtual const vec3& getPosition() const;

    void setTransform(const mat4& tr);
    const mat4& getTransform() const;
    const mat4& getGlobalTransform() const;

    void show();
    void hide();

    bool isVisible() const;
    bool hasChildren() const;

    virtual Type getType() const;

protected:
    virtual void calcLocalTransform();
    virtual void calcsUsingGlobal();

protected:
    mat4 m_globalTransform;
    mat4 m_localTransform;

    vec3 m_pos;
    quat m_rot;

    std::vector<SceneNode*> m_children;

    bool m_visible = true;
};
