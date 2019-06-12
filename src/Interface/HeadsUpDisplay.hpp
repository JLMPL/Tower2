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


namespace gfx
{
    class Renderer2D;
}

class RenderScene;
struct GameEvent;

namespace ui
{

class HeadsUpDisplay
{
public:
    void init(RenderScene& scene);
    void update();
    void draw();

    void show();
    void hide();

    void onEvent(const GameEvent& event);

private:
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

    bool m_isFocus = false;
    vec3 m_focusPos;
    gfx::Sprite m_focus;

    core::Timer m_timer;
    f32 m_healthPerc = 0.5f;
    f32 m_manaPerc = 0.5f;

    RenderScene* m_renderScene = nullptr;

    bool m_visible = true;

    friend class gfx::Renderer2D;
};

}
