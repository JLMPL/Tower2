#include "Cloth.hpp"
#include "Render/MeshManager.hpp"
#include "Core/Convert.hpp"
#include "Render/SceneRenderer.hpp"
#include "Animation/Animator.hpp"

namespace phys
{

constexpr u32 ClothProtectionFrames = 30;

Cloth::Cloth(physx::PxPhysics* phys, const std::string& mesh, anim::Animator* animer) :
    m_animator(animer)
{
    using namespace physx;
    m_mesh = gfx::g_MeshMgr.getSkinnedMesh(mesh, true);

    std::vector<PxClothParticle> verts;

    for (auto& vert : m_mesh->entries[0].vertices)
    {
        verts.push_back(PxClothParticle(
            core::conv::toPx(vec3(math::rotate(-90.0_rad, vec3(1,0,0)) * vec4(vert.pos, 1))),
            5));
    }

    physx::PxClothMeshDesc meshDesc;
    meshDesc.points.data = &verts[0];
    meshDesc.points.count = verts.size();
    meshDesc.points.stride = sizeof(PxClothParticle);

    meshDesc.invMasses.data = &verts[0].invWeight;
    meshDesc.invMasses.count = verts.size();
    meshDesc.invMasses.stride = sizeof(PxClothParticle);

    meshDesc.triangles.data = &m_mesh->entries[0].indices[0];
    meshDesc.triangles.count = m_mesh->entries[0].indices.size()/3;
    meshDesc.triangles.stride = sizeof(u32) * 3;

    if (!meshDesc.isValid())
        printf("NOT VALID!\n");

    PxClothMeshQuadifier quadifier(meshDesc);
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

    // m_cloth->setSelfCollisionDistance(0.002f);
    // m_cloth->setSelfCollisionStiffness(0.5f);

    m_cloth->setLinearInertiaScale(PxVec3(0.001f, 0.1f, 0.001f));

    m_cloth->setAngularInertiaScale(PxVec3(0.75));

    m_cloth->setStretchConfig(PxClothFabricPhaseType::eVERTICAL, PxClothStretchConfig(1));
    m_cloth->setStretchConfig(PxClothFabricPhaseType::eHORIZONTAL, PxClothStretchConfig(1));

    m_vertices = (vec3*)malloc(sizeof(vec3) * getVertexCount());
    m_constraints = (physx::PxClothParticleMotionConstraint*)malloc(sizeof(physx::PxClothParticleMotionConstraint) * getVertexCount());
}

Cloth::~Cloth()
{
    free(m_vertices);
    free(m_constraints);

    m_fabric->release();
    m_cloth->release();
}

void Cloth::skin()
{
    m_spawnTimer++;

    if (m_spawnTimer > ClothProtectionFrames + 1)
        m_spawnTimer = ClothProtectionFrames + 1;

    auto jTrs = m_animator->getMatrixPalette();

    auto& skinData = m_mesh->entries[0].weightsData;
    auto& vertData = m_mesh->entries[0].vertices;

    for (auto i = 0; i < vertData.size(); i++)
    {
        f32 rad = (m_spawnTimer > ClothProtectionFrames) ? vertData[i].color.r * 2 : 0;

        mat4 tr = (jTrs[skinData[i].joints[0]] * skinData[i].weights[0]) +
            (jTrs[skinData[i].joints[1]] * skinData[i].weights[1]) +
            (jTrs[skinData[i].joints[2]] * skinData[i].weights[2]) +
            (jTrs[skinData[i].joints[3]] * skinData[i].weights[3]);

        vec3 pos = vec3(tr * vec4(vertData[i].pos, 1));

        m_constraints[i] = physx::PxClothParticleMotionConstraint(core::conv::toPx(pos), rad);
    }

    m_cloth->setMotionConstraints(&m_constraints[0]);

    auto readData = m_cloth->lockParticleData();

    for (auto i = 0; i < getVertexCount(); i++)
    {
        m_vertices[i] = core::conv::toGlm(readData->particles[i].pos);
    }

    readData->unlock();
}

void Cloth::setCollisionSpheres(const Sphere* spheres, u32 numSpheres)
{
    m_cloth->setCollisionSpheres(&spheres[0], numSpheres);
}

void Cloth::addCapsule(i32 a, i32 b)
{
    m_cloth->addCollisionCapsule(a, b);
}

u32 Cloth::getVertexCount()
{
    m_cloth->getNbParticles();
}

vec3* Cloth::getVertices()
{
    return m_vertices;
}

void Cloth::setTargetTransform(const mat4& pos)
{
    m_cloth->setTargetPose(core::conv::toPx(pos));
}

gfx::Mesh* Cloth::getMesh() const
{
    return m_mesh;
}

physx::PxCloth* Cloth::getClothActor() const
{
    return m_cloth;
}

}
