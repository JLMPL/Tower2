#pragma once
#include "Render/2D/Rectangle.hpp"
#include "Render/2D/Text.hpp"

union SDL_Event;

class Console
{
public:
    Console() = default;

    void init();

    void sendSystemEvent(const SDL_Event& event);
    void drawGui();

    void toggle();

private:
    char m_buffer[128];

    bool m_visible = false;
};
