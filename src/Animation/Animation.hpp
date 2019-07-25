#pragma once
#include "Core/Math3D.hpp"
#include "Core/Name.hpp"
#include "Core/Json.hpp"
#include <memory>
#include <vector>
#include <functional>

namespace anim
{

template<typename T>
struct Key
{
    T value;
    f32 time;
};

struct JointAnimation
{
    core::Name32 name;

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
    core::Name32 name;
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

    core::Name32    name;
    mat4      offsetMatrix;
    i32       index = -1;
    i8        children[MaxChildren];
};

struct Skeleton
{
    std::vector<Joint> joints;
};

struct AnimationEvent
{
    f32 time;
    std::function<void (void)> func;
    bool alreadyFiredThisRun = false;
};

struct AnimationState
{
    using Ptr = std::unique_ptr<AnimationState>;

    std::string        name;

    const Animation*   anim = nullptr;
    const Skeleton*    skeleton = nullptr;

    std::vector<AnimationEvent> events;

    bool               isLooping = true;
    bool               hasRootMotion = false;
    vec3               rootMotion;

    f32                prevTime = 0_ms;
    f32                animTime = 0_ms;
    f32                lerpTime = 0_ms;
    bool               isLerp = false;

    Pose               startPose;
};

struct AnimationBundle
{
    std::string name;
    std::vector<AnimationState> states;
};

struct Animator
{
    const Skeleton*                  skeleton = nullptr;

    std::vector<mat4>                matrixPalette;
    std::vector<mat4>                jointTransforms;

    std::vector<AnimationState::Ptr> animStates;
    AnimationState*                  activeState = nullptr;

    Pose                             pose;
    vec3                             rootMotion;
};

void loadAnimationFromFile(Animation* anim, const std::string& path);
const JointAnimation* findJointInAnimation(const Animation& anim, const core::Name32& jointName);

Pose lerpPose(const Pose& a, const Pose& b, f32 v);

Pose getSkeletonPose(const Skeleton* skel, const Animation* anim, Seconds time);
vec3 getSkeletonRootMotion(const Skeleton* skel, const Animation* anim, Seconds time);

i8 findSkeletonJoint(const Skeleton* skel, const core::Name32& name);
Joint* getSkeletonJoint(Skeleton* skel, const core::Name32& name);

void initAnimationState(AnimationState* animState, const std::string& name, const Animation* anim, const Skeleton* skel);
void loadAnimStateFromJson(AnimationState* animState, const json& state);
void bindAnimStateEvent(AnimationState* animState, f32 time, const std::function<void (void)>& func);
void enterAnimationState(AnimationState* animState, Pose pose);
Pose updateAnimationState(AnimationState* animState, f32 delta);

void loadAnimationBundleFromFile(AnimationBundle* bundle, const std::string& path);

Animation* getLoadedAnimation(const std::string& name);
AnimationBundle* getLoadedBundle(const std::string& name);

void setupAnimator(Animator* animer, const Skeleton* skeleton, const AnimationBundle& anims);
Pose updateAnimator(Animator* animer);
AnimationState* getAnimatorState(Animator* animer, const std::string& str);
void setAnimatorState(Animator* animer, const std::string& str);
bool isAnimatorRootMotion(Animator* animer);

void updateAnimationSystem();
Animator* addAnimatorToAnimSystem(const Skeleton* skel, const std::string& bundle);

}
