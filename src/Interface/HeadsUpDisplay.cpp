#include "HeadsUpDisplay.hpp"
#include "Core/Config.hpp"
#include "Gameplay/Creature.hpp"
#include "Render/Camera.hpp"
#include "Render/FontManager.hpp"
#include "Render/Renderer2D.hpp"
#include "Render/TextureManager.hpp"

namespace ui
{

void HeadsUpDisplay::init()
{
    m_enemyHealth.init({128,16}, {1,0,0,1});

    m_lable.init(gfx::g_FontMgr.getFont("NotoSans-Regular.ttf"));
    m_lable.setCharacterSize(16);

    m_backLable.init(gfx::g_FontMgr.getFont("NotoSans-Regular.ttf"));
    m_backLable.setCharacterSize(16);
    m_backLable.setColor({0,0,0,1});

    auto display = core::g_Config.getDisplay();

    m_leftBack.init({64,64});
    m_leftBack.setPosition({16, display.height - (64 + 16)});
    m_leftBack.setColor({0,0,0,1});

    m_leftItem.init(gfx::g_TexMgr.getTexture("key.png"), {0,0,48,48});
    m_leftItem.setPosition(m_leftBack.getPosition() + vec2(8));

    m_healthBar.init({128,32}, {1,0,0,1});
    m_healthBar.setPosition({16 + 64, display.height - (16 + 64)});

    m_manaBar.init({128,32}, {0,0,1,1});
    m_manaBar.setPosition({16 + 64, display.height - (16 + 32)});
}

void HeadsUpDisplay::setCreature(Creature* creature)
{
    m_creature = creature;
}

void HeadsUpDisplay::update()
{
    if (auto focus = m_creature->getFocus(); focus && m_visible)
    {
        m_lable.setString(focus->getLabelName());
        m_backLable.setString(focus->getLabelName());

        auto display = core::g_Config.getDisplay();

        // vec3 flatpos = math::project(
        //     focus->getPos() + vec3(0, focus->getLabelOffset(), 0),
        //     gfx::g_Renderer3D.getActiveCamera()->getView(),
        //     gfx::g_Renderer3D.getActiveCamera()->getProjection(),
        //     vec4(0, 0, display.width, display.height)
        // );

        vec3 flatpos;

        m_lable.setPosition({
            i32(flatpos.x - m_lable.getWidth() * 0.5f),
            i32((display.height - flatpos.y) - m_lable.getHeight())
        });

        m_backLable.setPosition({
            i32(flatpos.x - m_backLable.getWidth() * 0.5f) + 2,
            i32((display.height - flatpos.y) - m_backLable.getHeight()) + 2
        });

        m_enemyHealth.setPosition({
            i32(flatpos.x - 64),
            i32((display.height - flatpos.y) + 10)
        });

        m_labelVisible = true;
    }
    else
        m_labelVisible = false;

    if (auto focus = m_creature->getFocus();
        m_labelVisible && focus->getType() == Entity::Type::Creature)
    {
        if (auto cre = focus->as<Creature>(); !cre->isDead())
        {
            m_enemyHealth.setMaxValue(cre->getMaxHealth());
            m_enemyHealth.setValue(cre->getHealth());
            m_lable.setColor({1,0,0,1});
            m_enemyHealthVisible = true;
        }
        else
            m_enemyHealthVisible = false;
    }
    else
    {
        m_enemyHealthVisible = false;
        m_lable.setColor({1,1,1,1});
    }

    if (m_visible)
    {
        m_healthBar.setMaxValue(m_creature->getMaxHealth());
        m_healthBar.setValue(m_creature->getHealth());

        m_manaBar.setMaxValue(m_creature->getMaxMagicka());
        m_manaBar.setValue(m_creature->getMagicka());
    }
}

void HeadsUpDisplay::draw()
{
    if (m_labelVisible)
    {
        gfx::g_Renderer2D.draw(m_backLable);
        gfx::g_Renderer2D.draw(m_lable);
    }

    if (m_enemyHealthVisible)
        m_enemyHealth.draw();

    // m_health.draw();
    // m_magicka.draw();

    gfx::g_Renderer2D.draw(m_leftBack);
    gfx::g_Renderer2D.draw(m_leftItem);
    m_healthBar.draw();
    m_manaBar.draw();
}

void HeadsUpDisplay::setCamera(const gfx::Camera* camera)
{
    m_camera = camera;
}

void HeadsUpDisplay::show()
{
    m_visible = true;
}

void HeadsUpDisplay::hide()
{
    m_visible = false;
}

}
