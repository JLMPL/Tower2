#include "Animation.hpp"
#include "Debug/Log.hpp"

namespace anim
{

std::vector<std::unique_ptr<Animator>> l_animators;

void updateAnimationSystem()
{
    for (auto& animator : l_animators)
        updateAnimator(animator.get());
}

Animator* addAnimatorToAnimSystem(const Skeleton* skel, const std::string& bundle)
{
    std::unique_ptr<Animator> animer(new Animator());
    setupAnimator(animer.get(), skel, *anim::getLoadedBundle(bundle));

    l_animators.push_back(std::move(animer));

    return l_animators.back().get();
}

}
