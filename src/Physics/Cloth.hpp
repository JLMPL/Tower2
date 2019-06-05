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
    ~Cloth();

    Cloth(physx::PxPhysics* phys, const std::string& mesh);

    void drawPoints();

    void setTargetTransform(const mat4& pos);

    void setConstraints(const std::vector<Constraint>& constraints);

    gfx::StaticMesh* getMesh() const;
    physx::PxCloth* getClothActor() const;

private:
    physx::PxClothFabric* m_fabric = nullptr;
    physx::PxCloth*       m_cloth = nullptr;

    gfx::StaticMesh* m_mesh = nullptr;
};

}
