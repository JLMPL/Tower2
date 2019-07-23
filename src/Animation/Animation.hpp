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

    bool hasRootMotion = true;
    JointAnimation rootMotion;

    f32 duration = 0_sec;
    bool loop = true;
};

void loadAnimationFromFile(Animation* anim, const std::string& path);
const JointAnimation* findJointInAnimation(const Animation& anim, const core::Name32& jointName);

struct JointPose
{
    core::Name32 name;

    vec3 pos;
    quat rot;
};

struct Pose
{
    std::vector<JointPose> jointPoses;
};

Pose lerpPose(const Pose& a, const Pose& b, f32 v);

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

class AnimationState
{
public:
    using Ptr = std::unique_ptr<AnimationState>;

    struct Event
    {
        f32 time;
        std::function<void (void)> func;
        bool alreadyFiredThisRun = false;
    };

    AnimationState() = default;
    AnimationState(const std::string& name, const Animation* anim, const Skeleton* skel);
    AnimationState(const json& state);

    void loadFromJson(const json& state);
    void setSkeleton(const Skeleton* skel);

    void enter(Pose pose);
    Pose update(f32 delta);
    void exit();

    void bindEvent(f32 time, const std::function<void (void)>& func);

    const std::string& getName() const;

    void setLoop(bool val);
    bool isLoop() const;

    f32 getDuration() const;

    void setHasRootMotion(bool val);
    bool hasRootMotion() const;
    vec3 getRootMotion() const;

    bool isFinished() const;

private:
    void resetEvents();
    void checkEvents();

private:
    std::string m_name;

    const Animation* m_anim = nullptr;
    const Skeleton* m_skeleton = nullptr;

    std::vector<Event> m_events;

    bool m_isLooping = true;
    bool m_hasRootMotion = false;
    vec3 m_rootMotion;

    f32 m_prevTime = 0_ms;
    f32 m_animTime = 0_ms;
    f32 m_lerpTime = 0_ms;
    bool m_isLerp = false;

    Pose m_startPose;
};

class AnimationBundle
{
public:
    AnimationBundle() = default;
    AnimationBundle(const std::string& path);
    ~AnimationBundle() = default;

    void loadFromFile(const std::string& path);

    const std::vector<AnimationState>& getStates() const;
    const std::string& getName() const;

private:
    std::string m_name;
    std::vector<AnimationState> m_states;
};

Animation* getLoadedAnimation(const std::string& name);
AnimationBundle* getLoadedBundle(const std::string& name);

struct Animator
{
    const Skeleton* skeleton = nullptr;

    std::vector<mat4> matrixPalette;
    std::vector<mat4> jointTransforms;

    std::vector<AnimationState::Ptr> animStates;
    AnimationState* activeState = nullptr;

    Pose pose;
    vec3 rootMotion;
};

void setupAnimator(Animator* animer, const Skeleton* skeleton, const AnimationBundle& anims);
Pose updateAnimator(Animator* animer);
AnimationState* getAnimatorState(Animator* animer, const std::string& str);
void setAnimatorState(Animator* animer, const std::string& str);
bool isAnimatorRootMotion(Animator* animer);

void updateAnimationSystem();
Animator* addAnimatorToAnimSystem(const Skeleton* skel, const std::string& bundle);

}
