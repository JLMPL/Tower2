#pragma once
#include "Interactible.hpp"
#include "Physics/PhysicsSystem.hpp"

struct GameEvent;
class SceneGraph;
class SceneNode;

class Chest : public Interactible
{
public:
    using Base = Entity;

    Chest(u32 id, LevelContext* context);

    void update() override final;
    void onEvent(const GameEvent& event);
    void interact(Creature* other) override final;

    Type getType() const override final;

private:
    SceneGraph* m_sceneGraph = nullptr;
    SceneNode* m_mesh = nullptr;
    phys::StaticBody m_staticBody;
};
