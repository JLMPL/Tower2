#pragma once
#include "SceneNode.hpp"

namespace gfx
{
    class SkinnedMesh;
}

namespace anim
{
    class Animator;
    class AnimationSystem;
}

class SkinnedMeshNode : public SceneNode
{
public:
    SkinnedMeshNode() = default;
    SkinnedMeshNode(anim::AnimationSystem* sys, gfx::SkinnedMesh* mesh, const std::string& bundle);
    ~SkinnedMeshNode() = default;

    void calcsUsingGlobal() override final;

    void attachNodeToJoint(i8 joint, SceneNode* node);
    void attachNodeToJoint(const std::string& joint, SceneNode* node);
    i8 getJointIndex(const std::string& name);

    gfx::SkinnedMesh* getMesh() const;
    anim::Animator* getAnimator() const;

    i32 getNumJoints() const;
    const mat4* getMatrixPalette() const;

    Type getType() const override final;

private:
    gfx::SkinnedMesh* m_mesh = nullptr;
    anim::Animator* m_anim = nullptr;

    struct JointChild
    {
        i8 joint = -1;
        SceneNode* node = nullptr;
    };

    std::vector<JointChild> m_jointChildren;
};
