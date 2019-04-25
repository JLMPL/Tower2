#pragma once
#include "Interactible.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Equipment.hpp"
#include "Interaction.hpp"

class SceneGraph;
class SceneNode;

class Chest : public Interactible
{
public:
    using Base = Entity;

    void init(u32 id, const std::string& code, SceneGraph* graph);
    void update() override final;
    void onEvent(const GameEvent& event) override final {}
    void interact(Creature* other) override final;

    Equipment& getEquipment();

    Type getType() const override final;

private:
    Equipment m_eq;
    // gfx::RenderableHandle m_mesh;
    SceneGraph* m_sceneGraph = nullptr;
    SceneNode* m_mesh = nullptr;
    phys::StaticBody m_staticBody;

    Interaction m_interaction;
};
