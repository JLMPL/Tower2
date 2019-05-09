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

    // m_level.initFromScript("Maps/Level0.lua");

    m_scene.initSystems();

    auto ent = m_scene.createEntity();

    auto tr = m_scene.addComponent<ecs::comp::Transform>(ent);
    tr->tr = mat4(1.f);

    auto mesh = m_scene.addComponent<ecs::comp::StaticMesh>(ent);
    mesh->mesh = nullptr;
}

void PlayingState::sendSystemEvent(const Event& event)
{
}

void PlayingState::update()
{
    g_EventSys.redistributeEvents();

    // m_level.update();

    m_scene.update();

    m_hud.update();
}

void PlayingState::draw()
{
    m_hud.draw();
}
