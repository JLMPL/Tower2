#pragma once
#include "SceneNode.hpp"

namespace gfx
{
    class SkinnedMesh;
}

namespace anim
{
    class Animator;
}

class SkinnedMeshNode : public SceneNode
{
public:
    SkinnedMeshNode() = default;
    SkinnedMeshNode(gfx::SkinnedMesh* mesh, const std::string& bundle);
    ~SkinnedMeshNode() = default;

    gfx::SkinnedMesh* getMesh() const;
    anim::Animator* getAnimator() const;

    i32 getNumJoints() const;
    const mat4* getMatrixPalette() const;

    Type getType() const override final;

private:
    gfx::SkinnedMesh* m_mesh = nullptr;
    anim::Animator* m_anim = nullptr;
};
