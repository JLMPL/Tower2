#include "Animation.hpp"
#include "Core/Convert.hpp"
#include "Debug/Log.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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

Pose getSkeletonPose(const Skeleton& skel, const Animation& anim, Seconds time)
{
    Pose poz;

    poz.jointPoses.resize(skel.joints.size());

    for (u32 i = 0; i < skel.joints.size(); i++)
    {
        const JointAnimation* jointAnim = findJointInAnimation(anim, skel.joints[i].name);

        if (jointAnim)
        {
            poz.jointPoses[i].name = skel.joints[i].name;

            poz.jointPoses[i].pos = lerpPosition(*jointAnim, time);
            poz.jointPoses[i].rot = lerpRotation(*jointAnim, time);
        }
    }

    return poz;
}

i8 findSkeletonJoint(const Skeleton* skel, const std::string& name)
{
    for (size_t i = 0; i < skel->joints.size(); i++)
    {
        if (skel->joints[i].name == name)
            return i;
    }
    return -1;
}

Joint* getSkeletonJoint(Skeleton* skel, const std::string& name)
{
    return &skel->joints[findSkeletonJoint(skel, name)];
}

LOCAL i8 addJointsToSkeleton(Skeleton& skeleton, const aiNode& node)
{
    i8 jointIndex = skeleton.joints.size();

    Joint joint;
    joint.name = node.mName.C_Str();

    std::string nome = joint.name.c_str();

    skeleton.joints.push_back(joint);
    skeleton.joints.back().index = skeleton.joints.size()-1;

    for (u32 i = 0; i < node.mNumChildren; i++)
    {
        i8 childJointIndex = addJointsToSkeleton(skeleton, *node.mChildren[i]);
        skeleton.joints[jointIndex].children.push_back(childJointIndex);
    }

    return jointIndex;
}

LOCAL void loadBindPose(Skeleton& skel, const aiScene& scene)
{
    for (int i = 0; i < scene.mNumMeshes; i++)
    {
        aiMesh* inMesh = scene.mMeshes[i];

        for (int i = 0; i < inMesh->mNumBones; i++)
        {
            // Joint* joint = A_getSkeletonJoint(skel, inMesh->mBones[i]->mName.data);
            Joint* joint = getSkeletonJoint(&skel, inMesh->mBones[i]->mName.data);

            joint->offsetMatrix = core::conv::toGlm(inMesh->mBones[i]->mOffsetMatrix);
            // memcpy(&joint->offsetMatrix, &inMesh->mBones[i]->mOffsetMatrix, sizeof(float) * 16);
        }
    }
}

void loadSkeletonFromFile(Skeleton* skel, const std::string& path)
{
    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(path.c_str(),
        aiProcess_LimitBoneWeights |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices
    );

    if (!scene)
    {
        Log::error("Could not load Mesh %s\n", path.c_str());
        return;
    }

    addJointsToSkeleton(*skel, *scene->mRootNode);
    loadBindPose(*skel, *scene);
}
