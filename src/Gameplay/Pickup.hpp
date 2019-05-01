#pragma once
#include "Interactible.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Item.hpp"

class Pickup : public Interactible
{
public:
    using Base = Entity;

    Pickup(u32 id);

    void init();
    void update() override final;

    void onEvent(const GameEvent& event) override final {}

    void addRigidBody(const vec3& pos = vec3(0,5,0));
    void addLight();

    void interact(Creature* other) override final;
    void disable() override final;

    phys::RigidBody& getRigidBody();

    Type getType() const;

private:
    const Item* m_item = nullptr;

    // gfx::RenderableHandle m_mesh;

    bool m_hasRigidBody = false;
    phys::RigidBody m_rigidBody;

    bool m_hasLight = false;
};
