#include "AnimationSystem.hpp"
#include "AnimationManager.hpp"
#include "Debug/Log.hpp"
#include "Pose.hpp"

namespace anim
{

void AnimationSystem::animate()
{
    for (auto& animator : m_animators)
        animator->update();
}

Animator* AnimationSystem::addAnimator(const Skeleton* skel, const std::string& bundle)
{
    m_animators.emplace_back(new Animator(skel, *anim::g_AnimMgr.getBundle(bundle)));
    return m_animators.back().get();
}

}
