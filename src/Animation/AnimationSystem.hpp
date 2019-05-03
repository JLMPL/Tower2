#pragma once
#include "Animator.hpp"
#include <memory>
#include <vector>

namespace anim
{

class Skeleton;

class AnimationSystem
{
public:
    AnimationSystem() = default;
    ~AnimationSystem() = default;

    void init();
    void animate();

    Animator* addAnimator(const Skeleton* skel, const std::string& bundle);

private:
    std::vector<std::unique_ptr<Animator>> m_animators;
};

}
