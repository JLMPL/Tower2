#pragma once
#include <Render/2D/FadedRect.hpp>
#include <Render/2D/Text.hpp>
#include <Core/Timer.hpp>
#include "EventSystem/EventListener.hpp"
#include <deque>

class Renderer2D;

namespace ui
{

class InfoPrompt : public EventListener
{
public:
    void init();
    void enqueue(const std::string& small, const std::string& big);

    void update();
    void draw();

private:
    void show();
    void hide();

    void onEvent(const GameEvent& event) override final;

private:
    struct Prompt
    {
        std::string small;
        std::string big;
    };

    gfx::FadedRect m_back;
    gfx::Text m_small;
    gfx::Text m_big;

    f32 m_timer = 0;

    bool m_visible = false;

    std::deque<Prompt> m_queue;
};

}
