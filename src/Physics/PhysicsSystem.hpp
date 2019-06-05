#pragma once
#include "Core/Convert.hpp"
#include "Core/FrameInfo.hpp"
#include "Core/Math3D.hpp"
#include "Core/Types.hpp"
#include "ErrorCallback.hpp"
#include "PhysicsAllocator.hpp"
#include "CCTFilterCallback.hpp"
#define _DEBUG
#include <PhysX/PxPhysicsAPI.h>
#include "RigidBody.hpp"
#include "StaticBody.hpp"
#include "Cloth.hpp"
#include <vector>

namespace phys
{

struct RaycastResult
{
    bool hasHit = false;
    vec3 pos;
    vec3 normal;
    i32 entityID = -1;
};

struct SweepResult
{
    bool hasHit = false;
    vec3 pos;
    vec3 normal;
};

class PhysicsSystem
{
public:
    PhysicsSystem() = default;
    PhysicsSystem(const PhysicsSystem&) = delete;
    PhysicsSystem& operator=(const PhysicsSystem&) = delete;
    ~PhysicsSystem();

    void init();
    void preSimulationUpdate();
    void stepSimulation();
    void debugDraw();

    void setupFilters(physx::PxRigidActor* actor, physx::PxU32 group, physx::PxU32 others);

    void addTriangleMesh(const physx::PxTriangleMeshDesc& desc);
    void addHeightField(const physx::PxHeightFieldDesc& desc, const vec3& scale);
    RigidBody addBox(const vec3& pos, const vec3& vel, const vec3& size);

    RigidBody addCapsule(const vec3& pos, const vec3& vel,
        f32 radius = 0.2, f32 halfHeight = 0.4);

    StaticBody addStaticBox(const vec3& size, const vec3& pos,
        const quat& rot = quat());

    void removeRigidBody(RigidBody& rb);

    physx::PxController* addController(u32* entityID, f32 radius, f32 height);

    Cloth* addCloth(const std::string& mesh);

    RaycastResult raycast(const vec3& origin, const vec3& dir, f32 dist);
    SweepResult sweepSphere(f32 radius, const vec3& origin, const vec3& dir, f32 dist);

private:
    physx::PxPhysics*                m_physics = nullptr;
    physx::PxFoundation*             m_foundation = nullptr;
    physx::PxCooking*                m_cooking = nullptr;
    ErrorCallback                    m_errorCallback;
    PhysicsAllocator                 m_allocCallback;

    CCTFilterCallback                m_cctFilterCallback;

    physx::PxScene*                  m_scene = nullptr;
    physx::PxMaterial*               m_material = nullptr;

    physx::PxControllerManager*      m_manager = nullptr;

    std::vector<physx::PxTriangleMesh*> m_triangleMeshes;
    std::vector<physx::PxRigidStatic*> m_statics;
    std::vector<physx::PxRigidDynamic*> m_dynamics;

    std::vector<Cloth::Ptr> m_cloths;

    physx::PxParticleSystem* m_particleSystem = nullptr;

    bool m_debugDraw = true;
};

}
