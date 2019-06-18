#pragma once
#include "Interactible.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Gameplay/Item.hpp"

class RenderMesh;

class Pickup : public Interactible
{
public:
    using Base = Entity;

    Pickup(u32 id, LevelContext* context);

    void init();
    void update() override final;

    void onEvent(const GameEvent& event) override final {}

    void addRigidBody(const vec3& pos = vec3(0,5,0));

    void interact(Creature* other) override final;

    void destroy() override final;

    phys::RigidBody& getRigidBody();

    Type getType() const;

private:
    const Item* m_item = nullptr;

    RenderMesh* m_mesh = nullptr;

    bool m_hasRigidBody = false;
    phys::RigidBody m_rigidBody;
};
