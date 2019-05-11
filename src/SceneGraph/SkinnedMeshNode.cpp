#include "SkinnedMeshNode.hpp"
#include "Animation/AnimationSystem.hpp"
#include "Render/Geometry/SkinnedMesh.hpp"

SkinnedMeshNode::SkinnedMeshNode(anim::AnimationSystem* sys, gfx::SkinnedMesh* mesh, const std::string& bundle) :
    m_mesh(mesh)
{
    m_anim = sys->addAnimator(&m_mesh->skeleton, bundle);
    m_anim->setState("Idle");
}

void SkinnedMeshNode::calcsUsingGlobal()
{
    for (auto& jc : m_jointChildren)
    {
        mat4 global = m_globalTransform * m_anim->getGlobalJointTransforms()[jc.joint];
        jc.node->updateTransforms(global);
    }
}

void SkinnedMeshNode::attachNode(i8 joint, SceneNode* node)
{
    m_jointChildren.push_back({joint, node});
}

void SkinnedMeshNode::attachNode(const std::string& joint, SceneNode* node)
{
    m_jointChildren.push_back({getJointIndex(joint), node});
}

i8 SkinnedMeshNode::getJointIndex(const std::string& name)
{
    return m_mesh->skeleton.findJointIndex(name.c_str());
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
