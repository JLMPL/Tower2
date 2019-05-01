#pragma once
#include "HeadsUpDisplay.hpp"
#include "InfoPrompt.hpp"
#include "ItemList.hpp"

namespace gfx
{
    class Renderer2D;
}

struct Event;
class Creature;

namespace ui
{

class Interface
{
public:
    Interface() = default;
    Interface(const Interface&) = delete;
    Interface& operator=(const Interface&) = delete;

    void init();
    void sendSystemEvent(const Event& event);
    void update();

    void draw();

    void setWhoms(Creature* creature);
    void prompt(const std::string& small, const std::string& big);

private:
    HeadsUpDisplay    m_hud;
    InfoPrompt        m_infoPrompt;
    ItemList          m_itemList;

    Creature*         m_creature = nullptr;
};

extern Interface g_Interface;

}
