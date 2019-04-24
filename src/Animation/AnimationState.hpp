#pragma once
#include "Animation.hpp"
#include <Core/Json.hpp>
#include "Pose.hpp"
#include <memory>
#include <map>

namespace anim
{

class Skeleton;

class AnimationState
{
public:
    using Ptr = std::unique_ptr<AnimationState>;

    struct Event
    {
        std::string name;
        f32 time;
        i32 joint;

        std::function<void (void)> func;

        bool alreadyFiredThisRun = false;
    };

    AnimationState() = default;
    AnimationState(const String& name, const Animation* anim, const Skeleton* skel);
    AnimationState(const json& state);

    void loadFromJson(const json& state);
    void setSkeleton(const Skeleton* skel);

    void enter(Pose pose);
    Pose update(f32 delta);
    void exit();

    void bindEvent(const std::string& evt, const std::function<void (void)>& func);

    // void addEvent(const std::string& name, f32 time, const std::string& joint);
    // bool poolEvent(Event& event);

    const String& getName() const;

    void setLoop(bool val);
    bool isLoop() const;

    f32 getDuration() const;

    void setHasRootMotion(bool val);
    bool hasRootMotion() const;
    vec3 getRootMotion() const;

    bool isFinished() const;

private:
    Event loadEventFromJson(const json& jvt);
    void resetEvents();
    void checkEvents();

private:
    String m_name;

    const Animation* m_anim = nullptr;
    const Skeleton* m_skeleton = nullptr;

    std::vector<Event> m_events;

    bool m_isLooping = true;
    bool m_hasRootMotion = false;
    vec3 m_rootMotion;

    f32 m_animTime = 0_ms;
    f32 m_lerpTime = 0_ms;
    bool m_isLerp = false;

    Pose m_startPose;
};

}
