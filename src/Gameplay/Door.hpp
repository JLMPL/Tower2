#pragma once
#include "Interactible.hpp"
#include "Physics/PhysicsSystem.hpp"

class Door : public Interactible
{
public:
    using Base = Entity;

    void init(u32 id, const std::string& code) override final;
    void update() override final;
    void onEvent(const GameEvent& event) override final;
    void interact(Creature* other) override final;

    void setKeyItem(const std::string& key);
    void open();

    Type getType() const override final;

private:
    enum DoorState
    {
        Closed,
        Opening,
        Open
    };

    void setDoorState(const DoorState& state);

private:
    DoorState m_state;
    f32 m_lerp = 0.f;
    f32 m_yaw = 0.f;

    std::string m_keyItem;

    // gfx::RenderableHandle m_renderableHandle;
    phys::StaticBody       m_staticBody;
};
