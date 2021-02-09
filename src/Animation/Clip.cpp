#include "Animation.hpp"
#include "Core/Convert.hpp"
#include "Debug/Log.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace anim
{

LOCAL void loadAnimation(Animation* anim, const std::string& path)
{
    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(path.c_str(), aiProcess_LimitBoneWeights);

    if (!scene)
    {
        Log::error("Could not load animation %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }

    if (scene->mNumAnimations > 0)
    {
        aiAnimation* animation = scene->mAnimations[0];

        for (u32 i = 0; i < animation->mNumChannels; i++)
        {
            aiNodeAnim* channel = animation->mChannels[i];

            JointAnimation jointAnimation;
            std::string str = channel->mNodeName.C_Str();
            jointAnimation.name = str.c_str();

            for (u32 i = 0; i < channel->mNumPositionKeys; i++)
            {
                Key<vec3> posKey;
                posKey.time = f32(channel->mPositionKeys[i].mTime);
                posKey.value = core::conv::toGlm(channel->mPositionKeys[i].mValue);
                jointAnimation.positionKeys.push_back(posKey);
            }

            for (u32 i = 0; i < channel->mNumRotationKeys; i++)
            {
                Key<quat> quatKey;
                quatKey.time = f32(channel->mRotationKeys[i].mTime);
                quatKey.value = core::conv::toGlm(channel->mRotationKeys[i].mValue);
                jointAnimation.rotationKeys.push_back(quatKey);
            }

            anim->jointAnimations.push_back(jointAnimation);
        }

        anim->duration = scene->mAnimations[0]->mDuration;
    }
    else
    {
        Log::error("Could not find any animations in file %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }
}

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

LOCAL vec3 lerpRoot(const JointAnimation& jointAnimation, Seconds animationTime)
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

LOCAL JointAnimation* findJointAnimInternal(Animation* anim, const std::string& jointName)
{
    for (auto& i : anim->jointAnimations)
        if (i.name == jointName)
            return &i;

    return nullptr;
}

void loadAnimationFromFile(Animation* anim, const std::string& path)
{
    loadAnimation(anim, path);
}

const JointAnimation* findJointInAnimation(const Animation& anim, const std::string& jointName)
{
    for (auto& i : anim.jointAnimations)
        if (i.name == jointName)
            return &i;

    return nullptr;
}

}
