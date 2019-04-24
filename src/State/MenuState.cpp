#include "MenuState.hpp"
#include "Render/Renderer2D.hpp"
#include "Render/GraphRenderer.hpp"
#include "Input/Input.hpp"
#include "Core/Config.hpp"
#include "StateStack.hpp"
#include <SDL2/SDL.h>

void MenuState::init()
{
    m_texture.loadFromFile("Textures/tmp_title_back2.png");
    m_sprite.init(&m_texture);
    m_sprite.setColor({0.5,0.5,0.5,1});

    auto display = core::g_Config.getDisplay();
    auto rect = m_sprite.getDest();

    f32 w = display.width / rect.w;
    f32 h = display.height / rect.h;

    m_sprite.setDest({0,0,rect.w * w, rect.h * h});

    m_font.loadFromFile("Fonts/NotoSans-Regular.ttf");

    m_title.init(&m_font);
    m_title.setCharacterSize(96);
    m_title.setString("TITLE");
    m_title.setPosition(
        (display.width / 2) - (m_title.getWidth() / 2),
        (display.height / 2) - m_title.getHeight()
    );

    m_text.init(&m_font);
    m_text.setCharacterSize(16);
    m_text.setString("Press almost any button!");
    m_text.setPosition(
        (display.width / 2) - (m_text.getWidth() / 2),
        (display.height / 2) + 64
    );
}

void MenuState::sendSystemEvent(const Event& event)
{
    if (event.type == Event::ButtonPressed)
    {
        m_stateStack.pop();
        m_stateStack.push(State::Playing);
    }
}

void MenuState::update()
{
    gfx::g_GraphRenderer.render(m_sceneGraph);
}

void MenuState::draw()
{
    gfx::g_Renderer2D.draw(m_sprite);
    gfx::g_Renderer2D.draw(m_title);
    gfx::g_Renderer2D.draw(m_text);
}
