#pragma once
#include "Physics/PhysicsSystem.hpp"

namespace phys
{

class CharacterController
{
public:
    CharacterController() = default;
    ~CharacterController();

    void init(PhysicsSystem* physSys, u32* entityID, f32 radius = 0.25, f32 height = 1.5);
    void move(const vec3& move, f32 deltaTime = 1000_ms, bool followGround = true);

    bool isOnGround() const;
    vec3 getGroundNormal() const;

    void setPosition(const vec3& pos);
    vec3 getPosition() const;

    vec3 getFootPosition() const;

private:
    PhysicsSystem* m_physSys = nullptr;
    physx::PxController* m_controller = nullptr;

    f32 m_radius = 1.f;
    f32 m_height = 1.f;

    bool m_onGround = false;
    vec3 m_groundNormal;
};

}
