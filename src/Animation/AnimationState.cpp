#include "Animation.hpp"

namespace anim
{

void initAnimationState(AnimationState* animState, const std::string& name, const Animation* anim, const Skeleton* skel)
{
    animState->name = name;
    animState->anim = anim;
    animState->skeleton = skel;
}

void loadAnimStateFromJson(AnimationState* animState, const json& state)
{
    animState->name = state["name"].get<std::string>();
    animState->anim = anim::getLoadedAnimation(state["file"]);

    animState->isLooping = state["loop"].get<bool>();
    animState->hasRootMotion = state["rootMotion"].get<bool>();
}

void bindAnimStateEvent(AnimationState* animState, f32 time, const std::function<void (void)>& func)
{
    animState->events.push_back({time, func, false});
}

void enterAnimationState(AnimationState* animState, Pose pose)
{
    animState->startPose = pose;
    animState->lerpTime = 0_ms;
    animState->animTime = 0_ms;
    animState->isLerp = true;

    for (auto& e : animState->events)
        e.alreadyFiredThisRun = false;
}

void checkEvents(AnimationState* animState)
{
    for (auto& e : animState->events)
    {
        if (animState->animTime >= e.time && !e.alreadyFiredThisRun)
        {
            if (e.func)
                e.func();

            e.alreadyFiredThisRun = true;
        }
    }
}

Pose updateAnimationState(AnimationState* animState, f32 delta)
{
    animState->prevTime = animState->animTime;
    animState->animTime += delta;

    if (animState->animTime >= animState->anim->duration)
    {
        animState->animTime = (animState->isLooping) ? 0_ms : animState->anim->duration;

        // printf("multiple resets\n");
        // resetEvents();
    }

    checkEvents(animState);

    if (animState->isLerp)
    {
        animState->lerpTime += delta * (1000.f/150.f);

        if (animState->lerpTime >= 1.f)
        {
            animState->lerpTime = 1;
            animState->isLerp = false;
        }

        Pose now = getSkeletonPose(animState->skeleton, animState->anim, animState->animTime);
        return lerpPose(animState->startPose, now, animState->lerpTime);
    }

    animState->rootMotion = getSkeletonRootMotion(animState->skeleton, animState->anim, animState->animTime) -
        getSkeletonRootMotion(animState->skeleton, animState->anim, animState->prevTime);

    return getSkeletonPose(animState->skeleton, animState->anim, animState->animTime);
}

}
