#include "Audio.hpp"
#include "Debug/Log.hpp"
#include "Core/Config.hpp"
#include <SDL2/SDL_mixer.h>

namespace aud
{

AudioSystem g_AudioSys;

AudioSystem::~AudioSystem()
{
    Mix_Quit();
}

void AudioSystem::init()
{
    // if (Mix_OpenAudioSystem(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    // {
    //     Log::error("Mixer initialization failed with error: %s\n", Mix_GetError());
    // }

    // m_newMagincia.loadFromFile("Music/new_magincia.mp3");
    // m_newMagincia.setVolume(Config::Audio::MusicVolume);

    // m_hurt.loadFromFile("Sounds/hurt.wav");
    // m_hurt.setVolume(Config::Audio::EffectsVolume);

    // m_swing.loadFromFile("Sounds/swing.wav");
    // m_swing.setVolume(Config::Audio::EffectsVolume);
}

void AudioSystem::refresh()
{
}

}
