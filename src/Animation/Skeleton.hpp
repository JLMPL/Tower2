#pragma once
#include "Animation.hpp"
#include "Pose.hpp"
#include "Core/Name.hpp"

namespace anim
{

struct Joint
{
    static constexpr u32 MaxChildren = 8u;

    core::Name32    name;
    mat4      offsetMatrix;
    i32       index = -1;
    i8        children[MaxChildren];
};

class Skeleton
{
public:
    Skeleton() = default;

    Pose getPose(const Animation* anim, Seconds time) const;
    vec3 getRootMotion(const Animation* anim, Seconds time) const;

    i8 findJointIndex(const core::Name32& name) const;
    Joint* findJoint(const core::Name32& name);

private:
    vec3 lerpPosition(const JointAnimation& jointAnimation, Seconds animationTime) const;
    quat lerpRotation(const JointAnimation& jointAnimation, Seconds animationTime) const;

public:
    std::vector<Joint> joints;
};

}
