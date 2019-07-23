#include "Animation.hpp"
// #include "AnimationBundle.hpp"

namespace anim
{

std::map<std::string, std::unique_ptr<Animation>> l_animations;
std::map<std::string, std::unique_ptr<AnimationBundle>> l_bundles;

Animation* getLoadedAnimation(const std::string& name)
{
    auto found = l_animations.find(name);

    if (found != l_animations.end())
        return (*found).second.get();

    std::unique_ptr<Animation> anim(new Animation());
    loadAnimationFromFile(anim.get(), std::string("Animations/Clips/" + name).c_str());
    l_animations[name] = std::move(anim);

    return l_animations[name].get();
}

AnimationBundle* getLoadedBundle(const std::string& name)
{
    auto found = l_bundles.find(name);

    if (found != l_bundles.end())
        return (*found).second.get();

    std::unique_ptr<AnimationBundle> bundle(new AnimationBundle());
    bundle->loadFromFile(std::string("Animations/Bundles/" + name).c_str());
    l_bundles[name] = std::move(bundle);

    return l_bundles[name].get();
}

}
