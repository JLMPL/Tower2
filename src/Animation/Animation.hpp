#pragma once
#include "Core/Math3D.hpp"
#include "Core/Name.hpp"
#include <memory>
#include <vector>

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
public:
    using Ptr = std::shared_ptr<Animation>;

    void loadFromFile(const std::string& path);

    const JointAnimation* findJointAnim(const core::Name32& jointName) const;

private:
    void loadAnimation(const std::string& path);

    vec3 lerpRoot(const JointAnimation& jointAnimation, Seconds animationTime) const;
    void generateRootMotion();

    JointAnimation* findJointAnimInternal(const core::Name32& jointName);

public:
    std::vector<JointAnimation> jointAnimations;

    bool hasRootMotion = true;
    JointAnimation rootMotion;

    f32 duration = 0_sec;
    bool loop = true;
};

}
