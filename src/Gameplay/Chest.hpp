#pragma once
#include "Interactible.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Interaction.hpp"

class SceneGraph;
class SceneNode;

class Chest : public Interactible
{
public:
    using Base = Entity;

    Chest(u32 id, LevelContext* context);

    void init();
    void update() override final;
    void onEvent(const GameEvent& event) override final {}
    void interact(Creature* other) override final;

    Type getType() const override final;

private:
    SceneGraph* m_sceneGraph = nullptr;
    SceneNode* m_mesh = nullptr;
    phys::StaticBody m_staticBody;

    Interaction m_interaction;
};
