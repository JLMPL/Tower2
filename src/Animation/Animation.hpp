#pragma once
#include "Core/Math3D.hpp"
#include "Core/Json.hpp"
#include <memory>
#include <vector>
#include <functional>

template<typename T>
struct Key
{
    T value;
    f32 time;
};

struct JointAnimation
{
    std::string name;

    std::vector<Key<vec3>> positionKeys;
    std::vector<Key<quat>> rotationKeys;
};

struct Animation
{
    using Ptr = std::shared_ptr<Animation>;

    std::vector<JointAnimation> jointAnimations;
    bool                        hasRootMotion = true;
    JointAnimation              rootMotion;
    f32                         duration = 0_sec;
    bool                        loop = true;
};

struct JointPose
{
    std::string name;
    vec3         pos;
    quat         rot;
};

struct Pose
{
    std::vector<JointPose> jointPoses;
};

struct Joint
{
    static constexpr u32 MaxChildren = 8u;

    std::string    name;
    mat4      offsetMatrix;
    i32       index = -1;

    std::vector<int> children;
};

struct Skeleton
{
    std::vector<Joint> joints;
};

void computeSkinMatrices(const Skeleton& skel, const Pose& pose, std::vector<mat4>& palette, std::vector<mat4>& globals);

void loadAnimationFromFile(Animation* anim, const std::string& path);
const JointAnimation* findJointInAnimation(const Animation& anim, const std::string& jointName);

Pose lerpPose(const Pose& a, const Pose& b, f32 v);

Pose getSkeletonPose(const Skeleton& skel, const Animation& anim, Seconds time);
vec3 getSkeletonRootMotion(const Skeleton* skel, const Animation* anim, Seconds time);

i8 findSkeletonJoint(const Skeleton* skel, const std::string& name);
Joint* getSkeletonJoint(Skeleton* skel, const std::string& name);

void loadSkeletonFromFile(Skeleton* skel, const std::string& path);
