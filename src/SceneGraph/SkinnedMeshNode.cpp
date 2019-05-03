#include "SkinnedMeshNode.hpp"
#include "Animation/AnimationSystem.hpp"
#include "Render/Geometry/SkinnedMesh.hpp"

SkinnedMeshNode::SkinnedMeshNode(anim::AnimationSystem* sys, gfx::SkinnedMesh* mesh, const std::string& bundle) :
    m_mesh(mesh)
{
    m_anim = sys->addAnimator(&m_mesh->skeleton, bundle);
    m_anim->setState("Idle");
}

gfx::SkinnedMesh* SkinnedMeshNode::getMesh() const
{
    return m_mesh;
}

anim::Animator* SkinnedMeshNode::getAnimator() const
{
    return m_anim;
}

i32 SkinnedMeshNode::getNumJoints() const
{
    return m_mesh->skeleton.joints.size();
}

const mat4* SkinnedMeshNode::getMatrixPalette() const
{
    return m_anim->getMatrixPalette();
}

SceneNode::Type SkinnedMeshNode::getType() const
{
    return Type::SkinMesh;
}
