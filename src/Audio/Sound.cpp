#include "Sound.hpp"
#include "Debug/Log.hpp"

namespace aud
{

Sound::~Sound()
{
    Mix_FreeChunk(m_chunk);
}

void Sound::loadFromFile(const std::string& path)
{
    m_chunk = Mix_LoadWAV(path.c_str());

    if (!m_chunk)
    {
        Log::error("Could not load sound %s with error: %s", path.c_str(), Mix_GetError());
    }
}

void Sound::play()
{
    i32 channel = Mix_PlayChannel(-1, m_chunk, 0);

    if (channel != -1)
        Mix_Volume(channel, m_volume);
}

void Sound::setVolume(u32 vol)
{
    m_volume = vol;
}

}
