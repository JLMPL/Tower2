#include "HeadsUpDisplay.hpp"
#include "Core/Config.hpp"
#include "Render/FontManager.hpp"
#include "Render/Renderer2D.hpp"
#include "Render/TextureManager.hpp"
#include "EventSystem/GameEvent.hpp"
#include "Render/Scene/RenderScene.hpp"

namespace ui
{

void HeadsUpDisplay::init(RenderScene& scene)
{
    m_renderScene = &scene;

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

    m_focus.init(gfx::g_TexMgr.getTexture("circle.png"));

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

void HeadsUpDisplay::onEvent(const GameEvent& event)
{
    if (event.getType() == GameEvent::Type::UpdateHud)
    {
        m_isFocus = event.hud.focus;

        m_focusPos.x = event.hud.focusX;
        m_focusPos.y = event.hud.focusY;
        m_focusPos.z = event.hud.focusZ;
    }
}

void HeadsUpDisplay::update()
{
    m_healthPerc = (sin(m_timer.getElapsedTime()) * 0.5f) + 0.5f;
    m_manaPerc = (sin(m_timer.getElapsedTime() + 0.5f) * 0.5f) + 0.5f;

    auto display = core::g_Config.getDisplay();

    vec3 flatpos = math::project(
        m_focusPos,
        m_renderScene->getView(),
        m_renderScene->getProjection(),
        vec4(0, 0, display.width, display.height)
    );

    // vec3 flatpos = vec3(100,100,0);

    m_focus.setPosition(vec2(i32(flatpos.x) -12, i32(display.height - flatpos.y) -12));

    // m_healthPerc = 0.8;
    // m_manaPerc = 0.75;
}

void HeadsUpDisplay::draw()
{
    if (m_isFocus)
        gfx::g_Renderer2D.draw(m_focus);

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
