#include "Interface.hpp"
#include <Events/Event.hpp>
#include "Gameplay/Creature.hpp"

namespace ui
{

Interface g_Interface;

void Interface::init()
{
    m_hud.init();
    m_infoPrompt.init();
    m_itemList.init();
}

void Interface::setCamera(const gfx::Camera* cam)
{
    m_hud.setCamera(cam);
}

void Interface::setWhoms(Creature* creature, Equipment* eq)
{
    m_creature = creature;
    m_eq = eq;

    m_hud.setCreature(m_creature);
}

void Interface::prompt(const std::string& small, const std::string& big)
{
    m_infoPrompt.enqueue(small, big);
}

void Interface::sendSystemEvent(const Event& event)
{
    m_itemList.sendSystemEvent(event);
}

void Interface::update()
{
    m_hud.update();
    m_infoPrompt.update();
    m_itemList.update();
}

void Interface::draw()
{
    m_infoPrompt.draw();
    m_hud.draw();
    m_itemList.draw();
}

void Interface::setWeapons(const std::vector<i32>& weaps)
{
    m_itemList.setItems(weaps);
}

}
