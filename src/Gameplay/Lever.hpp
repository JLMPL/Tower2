#pragma once
#include "Interactible.hpp"

class Lever final : public Interactible
{
public:
    using Base = Interactible;

    void init(u32 id, const std::string& code) override final;
    void update() override final;

    void setActivationTarget(u32 entityID);

    void onEvent(const GameEvent& event) override final {}
    void interact(Creature* other) override final;

    Type getType() const override final;

private:
    // gfx::RenderableHandle m_mesh;

    f32 m_pitch = 0.f;
    f32 m_lerp = 0.f;

    u32 m_activationTarget = 0;
    bool m_activated = false;
    bool m_flag = false;
};
