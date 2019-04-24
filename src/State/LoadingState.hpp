#pragma once
#include "State.hpp"
#include "Render/2D/Sprite.hpp"

class LoadingState : public State
{
public:
    LoadingState() = default;
    LoadingState(StateStack& ss) : State(ss) {}
    ~LoadingState() = default;

    void init() override final;
    void sendSystemEvent(const Event& event) override final;
    void update() override final;
    void draw() override final;

private:
    gfx::Sprite m_spr;
};
