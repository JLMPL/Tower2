#pragma once
#include "ThirdParty/entt/entt.hpp"
#include "Animation/Animation.hpp"

using namespace entt::literals;

struct AnimationLoader : entt::resource_loader<AnimationLoader, Animation>
{
    std::shared_ptr<Animation> load(const std::string& filename) const
    {
        auto anim = std::make_shared<Animation>();

        loadAnimationFromFile(anim.get(), filename);

        return anim;
    };
};

using _AnimationCache = entt::resource_cache<Animation>;
using AnimationHandle = entt::resource_handle<Animation>;

extern _AnimationCache AnimationCache;