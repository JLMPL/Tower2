#include "Animation.hpp"
#include <Core/Json.hpp>
#include <fstream>

namespace anim
{

void loadAnimationBundleFromFile(AnimationBundle* animBundle, const std::string& path)
{
    std::ifstream file(path.c_str());
    std::json bundle;
    file >> bundle;

    animBundle->name = bundle["name"];

    std::json& states = bundle["animations"];

    for (i32 i = 0; i < states.size(); i++)
    {
        AnimationState state;
        loadAnimStateFromJson(&state, states[i]);
        animBundle->states.push_back(state);
    }
}

}
