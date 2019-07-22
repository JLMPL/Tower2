/*#pragma once
#include "Music.hpp"
#include "Sound.hpp"

namespace aud
{

class AudioSystem
{
public:
    AudioSystem() = default;
    AudioSystem(const AudioSystem&) = delete;
    AudioSystem& operator=(const AudioSystem&) = delete;
    ~AudioSystem();

    void init();
    void refresh();

private:
    Music m_newMagincia;

    Sound m_hurt;
    Sound m_swing;
};

extern AudioSystem g_AudioSys;

}
*/
