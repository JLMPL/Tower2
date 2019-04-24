#pragma once
#include "HeadsUpDisplay.hpp"
#include "InfoPrompt.hpp"
#include "ItemList.hpp"

namespace gfx
{
    class Camera;
    class Renderer2D;
}

struct Event;
class Creature;
class Equipment;

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

    void setCamera(const gfx::Camera* cam);
    void setWhoms(Creature* creature, Equipment* eq);
    void prompt(const std::string& small, const std::string& big);

    void setWeapons(const std::vector<i32>& weaps);

private:
    HeadsUpDisplay    m_hud;
    InfoPrompt        m_infoPrompt;
    ItemList          m_itemList;

    Creature*         m_creature = nullptr;
    Equipment*        m_eq = nullptr;
};

extern Interface g_Interface;

}
