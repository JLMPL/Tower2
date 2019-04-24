#pragma once
#include "Render/2D/Text.hpp"
#include "Render/2D/Rectangle.hpp"
#include "Render/2D/Sprite.hpp"
#include "Bar.hpp"
#include "DiscreteBar.hpp"

namespace gfx
{
    class Camera;
}

class Creature;

namespace ui
{

class HeadsUpDisplay
{
public:
    void init();
    void setCreature(Creature* creature);
    void update();
    void draw();

    void setCamera(const gfx::Camera* camera);

    void show();
    void hide();

private:
    Creature*          m_creature = nullptr;
    const gfx::Camera* m_camera = nullptr;

    gfx::Rectangle     m_leftBack;
    gfx::Sprite        m_leftItem;
    Bar                m_healthBar;
    Bar                m_manaBar;

    Bar                m_enemyHealth;
    gfx::Text          m_lable;
    gfx::Text          m_backLable;

    bool               m_labelVisible = false;
    bool               m_enemyHealthVisible = false;
    bool               m_visible = true;
};

}
