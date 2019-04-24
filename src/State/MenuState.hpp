#pragma once
#include "State.hpp"
#include "Core/Timer.hpp"
#include "Render/2D/Font.hpp"
#include "Render/2D/Sprite.hpp"
#include "Render/2D/Text.hpp"
#include "Render/TargetCamera.hpp"
#include "Render/Texture.hpp"
#include "SceneGraph/SceneGraph.hpp"

class MenuState : public State
{
public:
    MenuState() = default;
    MenuState(StateStack& ss) : State(ss) {}
    ~MenuState() = default;

    void init() override final;
    void sendSystemEvent(const Event& event) override final;
    void update() override final;
    void draw() override final;

private:
    gfx::Texture m_texture;
    gfx::Sprite m_sprite;

    gfx::Font m_font;
    gfx::Text m_title;
    gfx::Text m_text;

    gfx::TargetCamera m_camera;

    SceneGraph m_sceneGraph;
};
