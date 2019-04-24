#pragma once
#include "Core/Math3D.hpp"
#include "Core/Name.hpp"
#include <vector>

namespace anim
{

struct JointPose
{
    core::Name32 name;

    vec3 pos;
    quat rot;
};

struct Pose
{
    std::vector<JointPose> jointPoses;

    static Pose lerp(const Pose& a, const Pose& b, f32 v);
};

}
