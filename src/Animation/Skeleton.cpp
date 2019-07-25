#include "Animation.hpp"

namespace anim
{

template<typename T>
LOCAL u32 findCurrentFrameIndex(const T& keys, f32 animationTime)
{
    for (u32 i = 0; i < keys.size() - 1; i++)
    {
        if (animationTime <= keys[i + 1].time)
            return i;
    }
    return -1;
}

LOCAL vec3 lerpPosition(const JointAnimation& jointAnimation, Seconds animationTime)
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

LOCAL quat lerpRotation(const JointAnimation& jointAnimation, Seconds animationTime)
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

Pose getSkeletonPose(const Skeleton* skel, const Animation* anim, Seconds time)
{
    Pose poz;

    poz.jointPoses.resize(skel->joints.size());

    for (u32 i = 0; i < skel->joints.size(); i++)
    {
        const JointAnimation* jointAnim = findJointInAnimation(*anim, skel->joints[i].name);

        if (jointAnim)
        {
            poz.jointPoses[i].name = skel->joints[i].name;

            poz.jointPoses[i].pos = lerpPosition(*jointAnim, time);
            poz.jointPoses[i].rot = lerpRotation(*jointAnim, time);
        }
    }

    return poz;
}

vec3 getSkeletonRootMotion(const Skeleton* skel, const Animation* anim, Seconds time)
{
    if (anim->hasRootMotion)
        return lerpPosition(anim->rootMotion, time);
    else
        return vec3(0);
}

i8 findSkeletonJoint(const Skeleton* skel, const core::Name32& name)
{
    for (size_t i = 0; i < skel->joints.size(); i++)
    {
        if (skel->joints[i].name == name)
            return i;
    }
    return -1;
}

Joint* getSkeletonJoint(Skeleton* skel, const core::Name32& name)
{
    return &skel->joints[findSkeletonJoint(skel, name)];
}

}
