#pragma once
#include "Core/Math3D.hpp"
#define _DEBUG
#include <PhysX/PxPhysicsAPI.h>
#include <memory>
#include <vector>

namespace gfx
{
    struct StaticMesh;
}

namespace phys
{

class Cloth
{
public:
    using Ptr = std::unique_ptr<Cloth>;

    struct Constraint
    {
        vec3 pos;
        f32 rad;

        Constraint(const vec3& p, f32 r) : pos(p), rad(r) {}
    };

    Cloth() = default;
    Cloth(physx::PxPhysics* phys, const std::string& mesh);
    ~Cloth();

    void setTargetTransform(const mat4& pos);

    void setConstraints(const std::vector<Constraint>& constraints);

    void setNumSpheres(u32 num);
    void setCollisionSphere(u32 index, const vec3& pos, f32 rad);
    void addCapsule(i32 a, i32 b);

    void lockParticleData();
    void unlockParticleData();

    u32 getVertexCount();
    vec3* getVertices();

    gfx::StaticMesh* getMesh() const;
    physx::PxCloth* getClothActor() const;

private:
    physx::PxClothFabric*  m_fabric = nullptr;
    physx::PxCloth*        m_cloth = nullptr;
    physx::PxClothMeshDesc m_meshDesc;

    vec3* m_vertices = nullptr;

    physx::PxClothParticleData* m_readData = nullptr;

    physx::PxClothCollisionSphere m_colSpheres[16];
    u32 m_numSpheres = 0;

    gfx::StaticMesh* m_mesh = nullptr;
};

}
