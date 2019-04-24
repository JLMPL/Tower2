#include "AnimationManager.hpp"
#include "AnimationBundle.hpp"

namespace anim
{

AnimationManager g_AnimMgr;

Animation* AnimationManager::getAnimation(const String& name)
{
    auto found = m_animations.find(name);

    if (found != m_animations.end())
        return (*found).second.get();

    std::unique_ptr<Animation> anim(new Animation());
    anim->loadFromFile(String("Animations/Clips/" + name).c_str());
    m_animations[name] = std::move(anim);

    return m_animations[name].get();
}

AnimationBundle* AnimationManager::getBundle(const std::string& name)
{
    auto found = m_bundles.find(name);

    if (found != m_bundles.end())
        return (*found).second.get();

    std::unique_ptr<AnimationBundle> bundle(new AnimationBundle());
    bundle->loadFromFile(String("Animations/Bundles/" + name).c_str());
    m_bundles[name] = std::move(bundle);

    return m_bundles[name].get();
}

}
