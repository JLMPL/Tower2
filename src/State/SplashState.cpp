#include "SplashState.hpp"
#include "Render/Renderer2D.hpp"
#include "Render/TextureManager.hpp"
#include "StateStack.hpp"
#include "Render/SceneRenderer.hpp"
#include "Core/Config.hpp"

void SplashState::init()
{
    m_spr.init(gfx::g_TexMgr.getTexture("physx_logo.png"));

    f32 w = 800.f / f32(m_spr.getDest().w);

    m_spr.setDest({0, 0, 800, m_spr.getDest().h * w});

    auto display = core::g_Config.getDisplay();
    m_spr.setPosition({
        display.halfWidth - (m_spr.getDest().w / 2),
        display.halfHeight - (m_spr.getDest().h / 2)
    });
}

void SplashState::sendSystemEvent(const Event& event)
{
    if (event.type == Event::Type::ButtonPressed)
    {
        m_stateStack.pop();
        m_stateStack.push(State::Menu);
    }
}

void SplashState::update()
{
    f32 alpha = sin(m_timer.getElapsedTime());

    m_spr.setColor({1,1,1,alpha});

    if (alpha <= 0)
    {
        m_stateStack.pop();
        m_stateStack.push(State::Menu);
    }

    gfx::g_SceneRenderer.render(m_renderScene);
}

void SplashState::draw()
{
    gfx::g_Renderer2D.draw(m_spr);
}
