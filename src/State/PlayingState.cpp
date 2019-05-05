#include "PlayingState.hpp"
#include "Render/Renderer2D.hpp"
#include "Input/Input.hpp"
#include "Gameplay/ItemManager.hpp"
#include "StateStack.hpp"
#include <SDL2/SDL.h>

void PlayingState::init()
{
    m_hud.init();
    g_ItemMgr.loadItems();

    m_level.initFromScript("Maps/Level0.lua");
}

void PlayingState::sendSystemEvent(const Event& event)
{
}

void PlayingState::update()
{
    g_EventSys.redistributeEvents();

    m_level.update();

    m_hud.update();
}

void PlayingState::draw()
{
    m_hud.draw();
}
