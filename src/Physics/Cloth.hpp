#pragma once
#include "Core/Math3D.hpp"
#define _DEBUG
#include <PhysX/PxPhysicsAPI.h>
#include <memory>
#include <vector>

namespace anim
{
    class Animator;
}

namespace gfx
{
    // struct StaticMesh;
    struct SkinnedMesh;
}

namespace phys
{

class Cloth
{
public:
    using Ptr = std::unique_ptr<Cloth>;
    using Sphere = physx::PxClothCollisionSphere;

    Cloth() = default;
    Cloth(physx::PxPhysics* phys, const std::string& mesh, anim::Animator* animer);
    ~Cloth();

    void setTargetTransform(const mat4& pos);

    void skin();

    void setCollisionSpheres(const Sphere* spheres, u32 numSpheres);
    void addCapsule(i32 a, i32 b);

    u32 getVertexCount();
    vec3* getVertices();

    gfx::SkinnedMesh* getMesh() const;
    physx::PxCloth* getClothActor() const;

private:
    physx::PxClothFabric*                   m_fabric = nullptr;
    physx::PxCloth*                         m_cloth = nullptr;

    physx::PxClothParticleMotionConstraint* m_constraints = nullptr;
    vec3*                                   m_vertices = nullptr;

    anim::Animator*                         m_animator = nullptr;
    gfx::SkinnedMesh*                       m_mesh = nullptr;
    u32                                     m_spawnTimer = 0;
};

}
