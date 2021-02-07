#pragma once
#include "ThirdParty/entt/entt.hpp"
#include "Animation/Animation.hpp"

using namespace entt::literals;

struct AnimationLoader : entt::resource_loader<AnimationLoader, anim::Animation>
{
    std::shared_ptr<anim::Animation> load(const std::string& filename) const
    {
        auto anim = std::make_shared<anim::Animation>();

        anim::loadAnimationFromFile(anim.get(), filename);

        return anim;
    };
};

using _AnimationCache = entt::resource_cache<anim::Animation>;
using AnimationHandle = entt::resource_handle<anim::Animation>;

extern _AnimationCache AnimationCache;