#pragma once
#include "State.hpp"
#include "Render/2D/Sprite.hpp"
#include "Core/Timer.hpp"
#include "SceneGraph/SceneGraph.hpp"

class SplashState : public State
{
public:
    SplashState() = default;
    SplashState(StateStack& ss) : State(ss) {}
    ~SplashState() = default;

    void init() override final;
    void sendSystemEvent(const Event& event) override final;
    void update() override final;
    void draw() override final;

private:
    gfx::Sprite m_spr;
    core::Timer m_timer;

    SceneGraph m_sceneGraph;
};
