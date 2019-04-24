#include "Animation.hpp"
#include <map>

namespace anim
{

class AnimationBundle;

class AnimationManager
{
public:
    AnimationManager() = default;
    ~AnimationManager() = default;

    Animation* getAnimation(const String& name);
    AnimationBundle* getBundle(const std::string& name);

private:
    std::map<String, std::unique_ptr<Animation>> m_animations;
    std::map<String, std::unique_ptr<AnimationBundle>> m_bundles;
};

extern AnimationManager g_AnimMgr;

}
