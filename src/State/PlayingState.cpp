#include "PlayingState.hpp"
#include "Render/Renderer2D.hpp"
#include "Input/Input.hpp"
#include "Interface/Interface.hpp"
#include "Gameplay/ItemManager.hpp"
#include "StateStack.hpp"
#include <SDL2/SDL.h>

void PlayingState::init()
{
    ui::g_Interface.init();
    g_ItemMgr.loadItems();

    m_level.loadFromFile("Maps/map0");
}

void PlayingState::sendSystemEvent(const Event& event)
{
    ui::g_Interface.sendSystemEvent(event);
}

void PlayingState::update()
{
    g_EventSys.redistributeEvents();

    m_level.update();
}

void PlayingState::lateUpdate()
{
    m_level.lateUpdate();
    ui::g_Interface.update();
}

void PlayingState::draw()
{
    ui::g_Interface.draw();
}
