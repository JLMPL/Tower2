#include "Cloth.hpp"
#include "Render/MeshManager.hpp"
#include "Core/Convert.hpp"
#include "Render/GraphRenderer.hpp"

namespace phys
{

Cloth::Cloth(physx::PxPhysics* phys, const std::string& mesh)
{
    using namespace physx;
    m_mesh = gfx::g_MeshMgr.getMesh(mesh, true);

    std::vector<PxClothParticle> verts;

    for (auto& vert : m_mesh->entries[0].vertices)
    {
        verts.push_back(PxClothParticle(core::conv::toPx(vert.pos), 5));
    }

    m_meshDesc.points.data = &verts[0];
    m_meshDesc.points.count = verts.size();
    m_meshDesc.points.stride = sizeof(PxClothParticle);

    m_meshDesc.invMasses.data = &verts[0].invWeight;
    m_meshDesc.invMasses.count = verts.size();
    m_meshDesc.invMasses.stride = sizeof(PxClothParticle);

    m_meshDesc.triangles.data = &m_mesh->entries[0].indices[0];
    m_meshDesc.triangles.count = m_mesh->entries[0].indices.size()/3;
    m_meshDesc.triangles.stride = sizeof(u32) * 3;

    if (!m_meshDesc.isValid())
        printf("NOT VALID!\n");

    PxClothMeshQuadifier quadifier(m_meshDesc);
    m_fabric = PxClothFabricCreate(*phys, quadifier.getDescriptor(), PxVec3(0, -1, 0));

    PxTransform pose = PxTransform(PxVec3(1,0.5,0));
    m_cloth = phys->createCloth(pose, *m_fabric, &verts[0], PxClothFlags());

    m_cloth->setClothFlag(PxClothFlag::eSCENE_COLLISION, false);
    m_cloth->setClothFlag(PxClothFlag::eSWEPT_CONTACT, false);

    m_cloth->setSolverFrequency(240.f);
    m_cloth->setStiffnessFrequency(5.0f);

    m_cloth->setDampingCoefficient(PxVec3(0.3f));
    m_cloth->setLinearDragCoefficient(PxVec3(0.2f));
    m_cloth->setAngularDragCoefficient(PxVec3(0.2f));

    m_cloth->setSelfCollisionDistance(0.002f);
    m_cloth->setSelfCollisionStiffness(0.5f);

/*    PxClothCollisionSphere spheres[2] =
    {
        PxClothCollisionSphere(PxVec3(0,2,0), 0.1),
        PxClothCollisionSphere(PxVec3(0,0,0), 0.1)
    };

    m_cloth->setCollisionSpheres(spheres, 2);
    m_cloth->addCollisionCapsule(0, 1);*/

    m_cloth->setLinearInertiaScale(PxVec3(0.001f, 0.01f, 0.001f));
    // m_cloth->setInertiaScale(0.01f);

    m_cloth->setAngularInertiaScale(PxVec3(0.75));

    // reduce centrifugal force of rotating frame
    // m_cloth->setCentrifugalInertiaScale(PxVec3(0.3f));

    m_cloth->setStretchConfig(PxClothFabricPhaseType::eVERTICAL, PxClothStretchConfig(1));
    m_cloth->setStretchConfig(PxClothFabricPhaseType::eHORIZONTAL, PxClothStretchConfig(1));

    m_vertices = (vec3*)malloc(sizeof(vec3) * getVertexCount());
}

Cloth::~Cloth()
{
    free(m_vertices);

    m_fabric->release();
    m_cloth->release();
}

void Cloth::setNumSpheres(u32 num)
{
    m_numSpheres = num;
}

void Cloth::setCollisionSphere(u32 index, const vec3& pos, f32 rad)
{
    m_colSpheres[index] = physx::PxClothCollisionSphere(core::conv::toPx(pos), rad);
    m_cloth->setCollisionSpheres(&m_colSpheres[0], m_numSpheres);
}

void Cloth::addCapsule(i32 a, i32 b)
{
    m_cloth->addCollisionCapsule(a, b);
}

void Cloth::lockParticleData()
{
    m_readData = m_cloth->lockParticleData();
}

void Cloth::unlockParticleData()
{
    if (m_readData)
    {
        m_readData->unlock();
        m_readData = nullptr;
    }
}

u32 Cloth::getVertexCount()
{
    m_cloth->getNbParticles();
}

vec3* Cloth::getVertices()
{
    for (auto i = 0; i < getVertexCount(); i++)
    {
        m_vertices[i] = core::conv::toGlm(m_readData->particles[i].pos);
    }

    return m_vertices;
}

void Cloth::setConstraints(const std::vector<Constraint>& constraints)
{
    std::vector<physx::PxClothParticleMotionConstraint> cut;

    for (auto i = 0; i < constraints.size(); i++)
    {
        cut.push_back(physx::PxClothParticleMotionConstraint(core::conv::toPx(constraints[i].pos), constraints[i].rad));
    }

    m_cloth->setMotionConstraints(&cut[0]);
}

void Cloth::setTargetTransform(const mat4& pos)
{
    m_cloth->setTargetPose(core::conv::toPx(pos));
}

gfx::StaticMesh* Cloth::getMesh() const
{
    return m_mesh;
}

physx::PxCloth* Cloth::getClothActor() const
{
    return m_cloth;
}

}
