#include "Skeleton.hpp"

namespace anim
{

template<typename T>
static u32 findCurrentFrameIndex(const T& keys, f32 animationTime)
{
    for (u32 i = 0; i < keys.size() - 1; i++)
    {
        if (animationTime <= keys[i + 1].time)
            return i;
    }
    return -1;
}

vec3 Skeleton::lerpPosition(const JointAnimation& jointAnimation, Seconds animationTime) const
{
    auto& keys = jointAnimation.positionKeys;

    if (keys.size() == 1)
        return keys[0].value;

    u32 currPosIndex = findCurrentFrameIndex(keys, animationTime);
    u32 nextPosIndex = (currPosIndex + 1);

    f32 difference = keys[nextPosIndex].time - keys[currPosIndex].time;
    f32 factor = (animationTime - keys[currPosIndex].time) / difference;

    return math::lerp(keys[currPosIndex].value, keys[nextPosIndex].value, factor);
}

quat Skeleton::lerpRotation(const JointAnimation& jointAnimation, Seconds animationTime) const
{
    auto& keys = jointAnimation.rotationKeys;

    if (keys.size() == 1)
        return keys[0].value;

    u32 currPosIndex = findCurrentFrameIndex(keys, animationTime);
    u32 nextPosIndex = (currPosIndex + 1);

    f32 difference = keys[nextPosIndex].time - keys[currPosIndex].time;
    f32 factor = (animationTime - keys[currPosIndex].time) / difference;

    return math::slerp(keys[currPosIndex].value, keys[nextPosIndex].value, factor);
}

Pose Skeleton::getPose(const Animation* anim, Seconds time) const
{
    Pose poz;

    poz.jointPoses.resize(joints.size());

    for (u32 i = 0; i < joints.size(); i++)
    {
        const JointAnimation* jointAnim = anim->findJointAnim(joints[i].name);

        if (jointAnim)
        {
            poz.jointPoses[i].name = joints[i].name;

            poz.jointPoses[i].pos = lerpPosition(*jointAnim, time);
            poz.jointPoses[i].rot = lerpRotation(*jointAnim, time);
        }
    }

    return poz;
}

vec3 Skeleton::getRootMotion(const Animation* anim, Seconds time) const
{
    if (anim->hasRootMotion)
        return lerpPosition(anim->rootMotion, time);
    else
        return vec3(0);
}

i8 Skeleton::findJointIndex(const core::Name32& name) const
{
    for (size_t i = 0; i < joints.size(); i++)
    {
        if (joints[i].name == name)
            return i;
    }
    return -1;
}

Joint* Skeleton::findJoint(const core::Name32& name)
{
    return &joints[findJointIndex(name)];
}

}
