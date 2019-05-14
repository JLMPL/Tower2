#pragma once
#include "Interactible.hpp"
#include "Physics/PhysicsSystem.hpp"

class SceneGraph;
class MeshNode;

class Door : public Entity
{
public:
    using Base = Entity;

    Door(u32 id, LevelContext* context);

    void update() override final;
    void onEvent(const GameEvent& event) override final;

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
    f32 m_raise = 0.f;

    MeshNode* m_mesh = nullptr;

    // gfx::RenderableHandle m_renderableHandle;
    phys::StaticBody       m_staticBody;
};
