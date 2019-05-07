#pragma once
#include "Render/2D/Text.hpp"
#include "Render/2D/Rectangle.hpp"
#include "Render/2D/Sprite.hpp"
#include "Render/Shader.hpp"
#include "Render/VertexArray.hpp"
#include "Render/ArrayBuffer.hpp"
#include "Bar.hpp"
#include "DiscreteBar.hpp"
#include "Core/Timer.hpp"

class Creature;

namespace gfx
{
    class Renderer2D;
}

namespace ui
{

class HeadsUpDisplay
{
public:
    void init();
    void setCreature(Creature* creature);
    void update();
    void draw();

    void show();
    void hide();

private:
    Creature*          m_creature = nullptr;

    gfx::Shader m_hudShader;
    gfx::Shader m_hudShader2;

    gfx::Texture m_circularMask;
    gfx::Texture m_healthMask;
    gfx::Texture m_manaMask;

    gfx::VertexArray m_hpVao;
    gfx::ArrayBuffer m_hpVbo;
    gfx::ArrayBuffer m_hpTbo;

    gfx::Sprite m_back;
    gfx::Sprite m_icon;

    core::Timer m_timer;
    f32 m_healthPerc = 0.5f;
    f32 m_manaPerc = 0.5f;

    bool m_visible = true;

    friend class gfx::Renderer2D;
};

}
