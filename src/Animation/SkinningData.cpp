#include "SkinningData.hpp"
#include "Debug/Log.hpp"
#include "Core/Timer.hpp"
#include "Render/SceneRenderer.hpp"

LOCAL void genMatrices(
    const Joint& joint,
    const mat4& parentTransform,
    const Pose& pose,
    const Skeleton& skeleton,
    std::vector<mat4>& palette,
    std::vector<mat4>& globals)
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

    if (joint.index < 0 || joint.index >= globals.size())
        return;

    globals[joint.index] = globalTransform;
    palette[joint.index] = globalTransform * joint.offsetMatrix;

    for (u32 i = 0; i < joint.children.size(); i++)
    {
        genMatrices(skeleton.joints[joint.children[i]], globalTransform, pose, skeleton, palette, globals);
    }
}

void computeSkinMatrices(const Skeleton& skel, const Pose& pose, std::vector<mat4>& palette, std::vector<mat4>& globals)
{
    genMatrices(skel.joints[0], mat4(1.f), pose, skel, palette, globals);
}

void SkinningData::computeMatrices(const Skeleton& skeleton, const Pose& pose)
{
    palette.resize(skeleton.joints.size());
    transforms.resize(skeleton.joints.size());
    genMatrices(skeleton.joints[0], mat4(1.f), pose, skeleton, palette, transforms);
}
