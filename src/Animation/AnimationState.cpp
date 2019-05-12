#include "AnimationState.hpp"
#include "AnimationManager.hpp"
#include "Skeleton.hpp"

namespace anim
{

AnimationState::AnimationState(
    const std::string& name, const Animation* anim, const Skeleton* skel)
    : m_name(name), m_anim(anim), m_skeleton(skel)
{
}

AnimationState::AnimationState(const json& state)
{
    loadFromJson(state);
}

void AnimationState::loadFromJson(const json& state)
{
    m_name = state["name"].get<std::string>();
    m_anim = anim::g_AnimMgr.getAnimation(state["file"]);

    m_isLooping = state["loop"].get<bool>();
    m_hasRootMotion = state["rootMotion"].get<bool>();

    // for (u32 i = 0; i < state["events"].size(); i++)
    // {
    //     loadEventFromJson(state["events"][i]);
    // }
}

AnimationState::Event AnimationState::loadEventFromJson(const json& jvt)
{
    // Event event;
    // event.name = jvt["type"].get<std::string>();
    // event.time = jvt["time"].get<f32>();
    // m_events.push_back(event);
}

void AnimationState::bindEvent(f32 time, const std::function<void (void)>& func)
{
    m_events.push_back({time, func, false});
}

void AnimationState::setSkeleton(const Skeleton* skel)
{
    m_skeleton = skel;
}

void AnimationState::enter(Pose pose)
{
    m_startPose = pose;
    m_lerpTime = 0_ms;
    m_animTime = 0_ms;
    m_isLerp = true;
    resetEvents();
}

void AnimationState::resetEvents()
{
    for (auto& e : m_events)
        e.alreadyFiredThisRun = false;
}

void AnimationState::checkEvents()
{
    for (auto& e : m_events)
    {
        if (m_animTime >= e.time && !e.alreadyFiredThisRun)
        {
            if (e.func)
                e.func();

            e.alreadyFiredThisRun = true;
        }
    }
}

Pose AnimationState::update(f32 delta)
{
    m_animTime += delta;

    if (m_animTime >= m_anim->duration)
    {
        m_animTime = (m_isLooping) ? 0_ms : m_anim->duration;

        // printf("multiple resets\n");
        // resetEvents();
    }

    checkEvents();

    if (m_isLerp)
    {
        m_lerpTime += delta * (1000.f/150.f);

        if (m_lerpTime >= 1.f)
        {
            m_lerpTime = 1;
            m_isLerp = false;
        }

        Pose now = m_skeleton->getPose(m_anim, m_animTime);
        return Pose::lerp(m_startPose, now, m_lerpTime);
    }

    m_rootMotion = m_skeleton->getRootMotion(m_anim, m_animTime);

    return m_skeleton->getPose(m_anim, m_animTime);
}

void AnimationState::exit()
{
}

const std::string& AnimationState::getName() const
{
    return m_name;
}

void AnimationState::setLoop(bool val)
{
    m_isLooping = val;
}

bool AnimationState::isLoop() const
{
    return m_isLooping;
}

f32 AnimationState::getDuration() const
{
    return m_anim->duration;
}

void AnimationState::setHasRootMotion(bool val)
{
    m_hasRootMotion = val;
}

bool AnimationState::hasRootMotion() const
{
    return m_hasRootMotion;
}

vec3 AnimationState::getRootMotion() const
{
    return m_rootMotion;
}

bool AnimationState::isFinished() const
{

}

}
