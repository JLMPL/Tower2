#include "HeadsUpDisplay.hpp"
#include "Core/Config.hpp"
#include "Gameplay/Creature.hpp"
#include "Render/FontManager.hpp"
#include "Render/Renderer2D.hpp"
#include "Render/TextureManager.hpp"

namespace ui
{

void HeadsUpDisplay::init()
{
    auto display = core::g_Config.getDisplay();

    m_hudShader.loadFromFile("Shaders/Interface/HudHealth.sha");
    m_hudShader2.loadFromFile("Shaders/Interface/HudMana.sha");

    m_circularMask.loadFromFile("Textures/circular_mask.png");
    m_healthMask.loadFromFile("Textures/health_mask.png");
    m_manaMask.loadFromFile("Textures/mana_mask.png");

    m_back.init(gfx::g_TexMgr.getTexture("health_back.png"));
    m_back.setColor(Color(0,0,0,0.8));

    m_back.setPosition(vec2(32, display.height - 160));

    m_icon.init(gfx::g_TexMgr.getTexture("fire_icon.png"));
    m_icon.setPosition(vec2(32, display.height - 160));

    const f32 verts[] =
    {
        0,   0,
        0,   128,
        128, 128,

        0,   0,
        128, 128,
        128, 0
    };
    const f32 coords[] =
    {
        0, 0,
        0, 1,
        1, 1,

        0, 0,
        1, 1,
        1, 0
    };

    m_hpVao.init();
    m_hpVao.bind();

    m_hpVbo.init(GL_ARRAY_BUFFER);
    m_hpVbo.bind();
    m_hpVbo.setData(sizeof(f32) * 2 * 6, verts, GL_STATIC_DRAW);
    m_hpVao.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_hpTbo.init(GL_ARRAY_BUFFER);
    m_hpTbo.bind();
    m_hpTbo.setData(sizeof(f32) * 2 * 6, coords, GL_STATIC_DRAW);
    m_hpVao.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_hpVao.unbind();
    m_hpVbo.unbind();
}

void HeadsUpDisplay::setCreature(Creature* creature)
{
    m_creature = creature;
}

void HeadsUpDisplay::update()
{
    m_healthPerc = (sin(m_timer.getElapsedTime()) * 0.5f) + 0.5f;
    m_manaPerc = (sin(m_timer.getElapsedTime() + 0.5f) * 0.5f) + 0.5f;

    // m_healthPerc = 0.8;
    // m_manaPerc = 0.75;
}

void HeadsUpDisplay::draw()
{
    gfx::g_Renderer2D.draw(m_back);
    gfx::g_Renderer2D.draw(*this);
    gfx::g_Renderer2D.draw(m_icon);
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
