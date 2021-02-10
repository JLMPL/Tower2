#include "Animation.hpp"

Pose lerpPose(const Pose& a, const Pose& b, f32 v)
{
    Pose final;
    final.jointPoses.resize(a.jointPoses.size());

    for (u32 i = 0; i < a.jointPoses.size(); i++)
    {
        final.jointPoses[i].name = a.jointPoses[i].name;
        final.jointPoses[i].pos = math::lerp(a.jointPoses[i].pos, b.jointPoses[i].pos, v);
        final.jointPoses[i].rot = math::slerp(a.jointPoses[i].rot, b.jointPoses[i].rot, v);
    }

    return final;
}
