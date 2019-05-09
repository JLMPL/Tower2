#pragma once
#include "Core/Types.hpp"
#include <SDL2/SDL_mixer.h>
#include <string>

namespace aud
{

class Music
{
public:
    Music() = default;
    ~Music();

    void loadFromFile(const std::string& path);

    void play();
    void pause();
    void resume();
    void halt();

    void setVolume(i32 volume);

private:
    Mix_Music* m_music = nullptr;
};

}
