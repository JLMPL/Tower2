#pragma once
#include "Interactible.hpp"
#include "Physics/PhysicsSystem.hpp"

class SceneGraph;
class SceneNode;

class Door : public Entity
{
public:
    using Base = Entity;

    Door(u32 id, LevelContext* context);

    void init();
    void update() override final;
    void onEvent(const GameEvent& event) override final;

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

    SceneGraph* m_sceneGraph = nullptr;
    SceneNode* m_mesh = nullptr;

    // gfx::RenderableHandle m_renderableHandle;
    phys::StaticBody       m_staticBody;
};
