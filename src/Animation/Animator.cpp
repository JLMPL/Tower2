#include "Animation.hpp"
#include "Debug/Log.hpp"
#include "Core/FrameInfo.hpp"

namespace anim
{

static mat4 DaeCorrectionMatrix = math::rotate(-90.0_rad, vec3(1,0,0));

void setupAnimator(Animator* animer, const Skeleton* skeleton, const AnimationBundle& anims)
{
    animer->skeleton = skeleton;

    animer->matrixPalette.resize(skeleton->joints.size());
    animer->jointTransforms.resize(skeleton->joints.size());

    for (u32 i = 0; i < anims.getStates().size(); i++)
    {
        AnimationState::Ptr state(new AnimationState());
        *state = anims.getStates()[i];
        state->setSkeleton(skeleton);
        animer->animStates.push_back(std::move(state));
    }
}

LOCAL void genGlobalPose(Animator* animer, const Joint& joint, const mat4& parentTransform, const Pose& pose)
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

    if (joint.index < 0 || joint.index >= animer->jointTransforms.size())
        return;

    animer->jointTransforms[joint.index] = DaeCorrectionMatrix * globalTransform;
    animer->matrixPalette[joint.index] = DaeCorrectionMatrix * globalTransform * joint.offsetMatrix;

    for (u32 i = 0; i < Joint::MaxChildren; i++)
    {
        if (joint.children[i] != -1)
            genGlobalPose(animer, animer->skeleton->joints[joint.children[i]], globalTransform, pose);
    }
}

Pose updateAnimator(Animator* animer)
{
    animer->pose = animer->activeState->update(core::g_FInfo.delta);
    animer->rootMotion = animer->activeState->getRootMotion();

    genGlobalPose(animer, animer->skeleton->joints[0], mat4(1.f), animer->pose);

    return animer->pose;
}

AnimationState* getAnimatorState(Animator* animer, const std::string& str)
{
    for (auto& state : animer->animStates)
    {
        if (state->getName() == str)
            return state.get();
    }

    return nullptr;
}

void setAnimatorState(Animator* animer, const std::string& str)
{
    auto state = getAnimatorState(animer, str);

    if (animer->activeState == state)
        return;

    if (!state)
    {
        Log::warning("Could not set state %s does not exist!\n", str.c_str());
        return;
    }

    state->enter(animer->pose);
    animer->activeState = state;
}

bool isAnimatorRootMotion(Animator* animer)
{
    return animer->activeState->hasRootMotion();
}

}
