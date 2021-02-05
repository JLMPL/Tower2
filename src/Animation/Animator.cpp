#include "Animation.hpp"
#include "Debug/Log.hpp"
#include "Core/Timer.hpp"
#include "Render/SceneRenderer.hpp"

namespace anim
{

static mat4 DaeCorrectionMatrix = mat4(1.f);//math::rotate(-90.0_rad, vec3(1,0,0));

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

    globals[joint.index] = DaeCorrectionMatrix * globalTransform;
    gfx::g_SceneRenderer.addLine((DaeCorrectionMatrix * parentTransform)[3], globals[joint.index][3], vec3(0,1,0));
    palette[joint.index] = DaeCorrectionMatrix * globalTransform * joint.offsetMatrix;

    for (u32 i = 0; i < Joint::MaxChildren; i++)
    {
        if (joint.children[i] != -1)
            genMatrices(skeleton.joints[joint.children[i]], globalTransform, pose, skeleton, palette, globals);
    }
}

void computeSkinMatrices(const Skeleton& skel, const Pose& pose, std::vector<mat4>& palette, std::vector<mat4>& globals)
{
    genMatrices(skel.joints[0], mat4(1.f), pose, skel, palette, globals);
}

}
