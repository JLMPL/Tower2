#include "AnimationSystem.hpp"
#include "AnimationManager.hpp"
#include "Debug/Log.hpp"
#include "Pose.hpp"

namespace anim
{

AnimationSystem g_AnimSys;

void AnimationSystem::init()
{
}

void AnimationSystem::animate()
{
    for (auto& animator : m_animators)
        animator->update();
}

Animator* AnimationSystem::addAnimator(const Skeleton* skel, const std::string& bundle)
{
    m_animators.emplace_back(new Animator());
    Animator* ar = m_animators.back().get();

    if (!ar)
        Log::warning("AnimationSystem: Could not add Animator! - Out of memory!\n");



    ar->init(skel, *anim::g_AnimMgr.getBundle(bundle));
    return ar;
}

}
