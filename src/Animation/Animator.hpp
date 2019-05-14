#pragma once
#include <Core/FrameInfo.hpp>
#include "AnimationBundle.hpp"
#include <functional>

namespace anim
{

class Skeleton;
class Animation;
struct Joint;

class Animator
{
public:
    Animator() = default;
    Animator(const Skeleton* skeleton, const AnimationBundle& anims);
    ~Animator() = default;

    Pose update();

    AnimationState* addState(const std::string& str, const Animation* anim);
    AnimationState* getState(const std::string& str);

    void setState(const std::string& str);
    void setStateOnGlobalFrame(const std::string& str, u64 globalFrame);
    void setStateOnGlobalTime(const std::string& str, f64 globalTime);
    void callFunctionOnGlobalTime(const std::function<void (void)>& func, f64 globalTime);

    void clearPendingFunctions();

    AnimationState* getCurrentState() const;

    bool isRootMotion() const;
    vec3 getRootMotion() const;

    const Skeleton& getSkeleton() const;
    const mat4* getMatrixPalette() const;
    const mat4* getGlobalJointTransforms() const;

private:
    void genGlobalPose(const Joint& joint, const mat4& parentTransform, const Pose& pose);

    void checkForPendingStates();
    void checkForPendingFunctions();
    void mixStates();

private:
    struct PendingState
    {
        u64 frame = 0;
        f64 time = 0;
        AnimationState* state = nullptr;
    };

    struct PendingFunction
    {
        u64 frame = 0;
        f64 time = 0;
        std::function<void (void)> func;
    };

    const Skeleton* m_skeleton = nullptr;

    std::vector<mat4> m_matrixPalette;
    std::vector<mat4> m_globalTransforms;

    std::vector<AnimationState::Ptr> m_animStates;
    std::vector<AnimationState*> m_activeStates;
    std::vector<PendingState> m_pendingStates;

    std::vector<PendingFunction> m_pendingFunctions;

    Pose m_pose;
    vec3 m_rootMotion;
};

}
