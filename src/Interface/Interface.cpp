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
    // m_itemList.init();
}

void Interface::setWhoms(Creature* creature)
{
    m_creature = creature;

    m_hud.setCreature(m_creature);
}

void Interface::prompt(const std::string& small, const std::string& big)
{
    m_infoPrompt.enqueue(small, big);
}

void Interface::sendSystemEvent(const Event& event)
{
    // m_itemList.sendSystemEvent(event);
}

void Interface::update()
{
    m_hud.update();
    m_infoPrompt.update();
    // m_itemList.update();
}

void Interface::draw()
{
    m_infoPrompt.draw();
    m_hud.draw();
    // m_itemList.draw();
}

}
