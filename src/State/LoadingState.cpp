#include "LoadingState.hpp"
#include "Render/Renderer2D.hpp"
#include "Render/TextureManager.hpp"

void LoadingState::init()
{
    m_spr.init(gfx::g_TexMgr.getTexture("error.png"), {0,0,512,512}, {0,0,256,256});
}

void LoadingState::sendSystemEvent(const Event& event)
{

}

void LoadingState::update()
{

}

void LoadingState::draw()
{
    gfx::g_Renderer2D.draw(m_spr);
}
