#include "Animator.hpp"
#include "Debug/Log.hpp"
#include "Animation.hpp"
#include "Pose.hpp"
#include "Skeleton.hpp"

namespace anim
{

static mat4 DaeCorrectionMatrix = math::rotate(-90.0_rad, vec3(1,0,0));

Animator::Animator(const Skeleton* skeleton, const AnimationBundle& anims)
{
    m_skeleton = skeleton;

    m_matrixPalette.resize(skeleton->joints.size());
    m_globalTransforms.resize(skeleton->joints.size());

    for (u32 i = 0; i < anims.getStates().size(); i++)
    {
        AnimationState::Ptr state(new AnimationState());
        *state = anims.getStates()[i];
        state->setSkeleton(skeleton);
        m_animStates.push_back(std::move(state));
    }
}

void Animator::genGlobalPose(const Joint& joint, const mat4& parentTransform, const Pose& pose)
{
    const JointPose* poze = nullptr;

    for (auto& jp : pose.jointPoses)
    {
        if (jp.name == joint.name)
            poze = &jp;
    }

    mat4 jointTransformation;

    if (poze)
    {
        mat4 translation = math::translate(poze->pos);
        mat4 orientation = math::mat4_cast(poze->rot);

        jointTransformation = translation * orientation;
    }

    mat4 globalTransform = parentTransform * jointTransformation;

    if (joint.index < 0 || joint.index >= m_globalTransforms.size())
        return;

    m_globalTransforms[joint.index] = DaeCorrectionMatrix * globalTransform;
    m_matrixPalette[joint.index] = DaeCorrectionMatrix * globalTransform * joint.offsetMatrix;

    for (u32 i = 0; i < Joint::MaxChildren; i++)
    {
        if (joint.children[i] != -1)
            genGlobalPose(m_skeleton->joints[joint.children[i]], globalTransform, pose);
    }
}

Pose Animator::update()
{
    m_pose = m_activeState->update(core::g_FInfo.delta);
    m_rootMotion = m_activeState->getRootMotion();

    genGlobalPose(m_skeleton->joints[0], mat4(1.f), m_pose);

    return m_pose;
}

AnimationState* Animator::getState(const std::string& str)
{
    for (auto& state : m_animStates)
    {
        if (state->getName() == str)
            return state.get();
    }

    return nullptr;
}

void Animator::setState(const std::string& str)
{
    auto state = getState(str);

    if (m_activeState == state)
        return;

    if (!state)
    {
        Log::warning("Could not set state %s does not exist!\n", str.c_str());
        return;
    }

    state->enter(m_pose);
    m_activeState = state;
}

AnimationState* Animator::getCurrentState() const
{
    return m_activeState;
}

bool Animator::isRootMotion() const
{
    return m_activeState->hasRootMotion();
}

vec3 Animator::getRootMotion() const
{
    return m_rootMotion;
}

const Skeleton& Animator::getSkeleton() const
{
    return *m_skeleton;
}

const mat4* Animator::getMatrixPalette() const
{
    return &m_matrixPalette[0];
}

const mat4* Animator::getGlobalJointTransforms() const
{
    return &m_globalTransforms[0];
}

}
