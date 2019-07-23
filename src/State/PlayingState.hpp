#pragma once
#include "State.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Gameplay/Level.hpp"

class PlayingState : public State
{
public:
    PlayingState() = default;
    PlayingState(StateStack& ss) : State(ss) {}

    void init() override final;
    void sendSystemEvent(const Event& event) override final;
    void update() override final;
    void draw() override final;

private:
    // ui::HeadsUpDisplay m_hud;
    Level          m_level;
};
