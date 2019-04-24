#include "AnimationBundle.hpp"
#include <Core/Json.hpp>
#include <fstream>

namespace anim
{

AnimationBundle::AnimationBundle(const std::string& path)
{
    loadFromFile(path);
}

void AnimationBundle::loadFromFile(const std::string& path)
{
    std::ifstream file(path.c_str());
    json bundle;
    file >> bundle;

    m_name = bundle["name"];

    json& states = bundle["animations"];

    for (i32 i = 0; i < states.size(); i++)
    {
        m_states.push_back(AnimationState(states[i]));
    }
}

const std::vector<AnimationState>& AnimationBundle::getStates() const
{
    return m_states;
}

const std::string& AnimationBundle::getName() const
{
    return m_name;
}

}
