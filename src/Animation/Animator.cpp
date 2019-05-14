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
        (*state).setSkeleton(skeleton);
        m_animStates.push_back(std::move(state));
    }
}

void Animator::checkForPendingStates()
{
    if (m_pendingStates.empty())
        return;

    for (i32 i = m_pendingStates.size()-1; i >= 0; i--)
    {
        auto& pending = m_pendingStates[i];

        if (pending.frame == core::g_FInfo.globalFrame ||
            (pending.time != 0.0 && core::g_FInfo.globalTime >= pending.time))
        {
            pending.state->enter(m_pose);
            m_activeStates.push_back(pending.state);
            m_pendingStates.erase(m_pendingStates.begin() + i);
        }
    }
}

void Animator::checkForPendingFunctions()
{
    if (m_pendingFunctions.empty())
        return;

    for (i32 i = m_pendingFunctions.size()-1; i >= 0; i--)
    {
        auto& pending = m_pendingFunctions[i];

        if (pending.frame == core::g_FInfo.globalFrame ||
            (pending.time != 0.0 && core::g_FInfo.globalTime >= pending.time))
        {
            pending.func();
            m_pendingFunctions.erase(m_pendingFunctions.begin() + i);
        }
    }
}

void Animator::mixStates()
{
    for (auto& i : m_activeStates)
    {
        m_pose = i->update(core::g_FInfo.delta);
        m_rootMotion = i->getRootMotion();
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
    if (m_activeStates.empty())
        return m_pose;

    checkForPendingStates();
    checkForPendingFunctions();
    mixStates();
    genGlobalPose(m_skeleton->joints[0], mat4(1.f), m_pose);

    return m_pose;
}

AnimationState* Animator::addState(const std::string& str, const Animation* anim)
{
    AnimationState::Ptr state(new AnimationState(str, anim, m_skeleton));
    m_animStates.push_back(std::move(state));
    return m_animStates.back().get();
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

    if (!m_activeStates.empty())
        if (m_activeStates.back() == state)
            return;

    m_activeStates.clear();


    if (!state)
    {
        Log::warning("Could not set state %s does not exist!\n", str.c_str());
        return;
    }

    state->enter(m_pose);
    m_activeStates.push_back(state);
}

void Animator::setStateOnGlobalFrame(const std::string& str, u64 globalFrame)
{
    auto state = getState(str);

    if (!state)
    {
        Log::warning("Could not queue(frame) state %s does not exist!\n", str.c_str());
        return;
    }

    PendingState ps;
    ps.frame = globalFrame;
    ps.state = state;

    m_pendingStates.push_back(ps);
}

void Animator::setStateOnGlobalTime(const std::string& str, f64 globalTime)
{
    auto state = getState(str);

    if (!state)
    {
        Log::warning("Could not queue(frame) state %s does not exist!\n", str.c_str());
        return;
    }

    PendingState ps;
    ps.time = globalTime;
    ps.state = state;

    m_pendingStates.push_back(ps);
}

void Animator::callFunctionOnGlobalTime(const std::function<void (void)>& func, f64 globalTime)
{
    PendingFunction pf;

    pf.time = globalTime;
    pf.func = func;

    m_pendingFunctions.push_back(pf);
}

void Animator::clearPendingFunctions()
{
    m_pendingFunctions.clear();
}

AnimationState* Animator::getCurrentState() const
{
    return m_activeStates[0];
}

bool Animator::isRootMotion() const
{
    return m_activeStates[0]->hasRootMotion();
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
