#include "Music.hpp"
#include "Debug/Log.hpp"

namespace aud
{

Music::~Music()
{
    Mix_FreeMusic(m_music);
}

void Music::loadFromFile(const Path& path)
{
    m_music = Mix_LoadMUS(path.c_str());

    if (!m_music)
    {
        Log::error("Could not load music %s with error: %s", path.c_str(), Mix_GetError());
    }
}

void Music::play()
{
    Mix_PlayMusic(m_music, -1);
}

void Music::pause()
{
    if (Mix_PausedMusic() == 0)
        Mix_PauseMusic();
}

void Music::resume()
{
    if (Mix_PausedMusic() == 1)
        Mix_ResumeMusic();
}

void Music::halt()
{
    Mix_HaltMusic();
}

void Music::setVolume(i32 volume)
{
    Mix_VolumeMusic(volume);
}

}
