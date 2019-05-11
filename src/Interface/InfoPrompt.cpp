#include "InfoPrompt.hpp"
#include "Render/Renderer2D.hpp"
#include "Render/FontManager.hpp"
#include "Core/Config.hpp"
#include "Core/FrameInfo.hpp"
#include "EventSystem/EventSystem.hpp"

namespace ui
{

void InfoPrompt::init()
{
    m_back.init({192,80});
    m_back.setColor({0,0,0,0.8});
    m_back.setFade(128);

    auto display = core::g_Config.getDisplay();

    m_back.setPosition(0, display.halfHeight - 32);

    m_small.init(gfx::g_FontMgr.getFont("DejaVuSans-Bold.ttf"));
    m_small.setCharacterSize(12);
    m_small.setPosition(32, display.halfHeight - 24);

    m_big.init(gfx::g_FontMgr.getFont("DejaVuSans-Bold.ttf"));
    m_big.setCharacterSize(24);
    m_big.setPosition(32, display.halfHeight);
}

void InfoPrompt::enqueue(const std::string& small, const std::string& big)
{
    m_queue.push_back({small, big});
    m_timer = 0;
}

void InfoPrompt::onEvent(const GameEvent& event)
{
    switch (event.getType())
    {
    }
}

void InfoPrompt::update()
{
    if (m_queue.empty())
        hide();
    else
    {
        show();

        m_timer += core::g_FInfo.delta;

        m_small.setString(m_queue[0].small);
        m_big.setString(m_queue[0].big);

        if (m_timer > 1)
        {
            m_queue.pop_front();
            m_timer = 0;
        }
    }
}

void InfoPrompt::draw()
{
    if (m_visible)
    {
        gfx::g_Renderer2D.draw(m_back);
        gfx::g_Renderer2D.draw(m_small);
        gfx::g_Renderer2D.draw(m_big);
    }
}

void InfoPrompt::show()
{
    m_visible = true;
}

void InfoPrompt::hide()
{
    m_visible = false;
}

}
