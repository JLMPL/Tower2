#pragma once
#include "AnimationState.hpp"

namespace anim
{

class AnimationBundle
{
public:
    AnimationBundle() = default;
    AnimationBundle(const std::string& path);
    ~AnimationBundle() = default;

    void loadFromFile(const std::string& path);

    const std::vector<AnimationState>& getStates() const;
    const std::string& getName() const;

private:
    std::string m_name;
    std::vector<AnimationState> m_states;
};

}
