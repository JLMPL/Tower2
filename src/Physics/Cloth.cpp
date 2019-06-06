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
    std::vector<PxClothParticleMotionConstraint> constraints;

    for (auto& vert : m_mesh->entries[0].vertices)
    {
        verts.push_back(PxClothParticle(core::conv::toPx(vert.pos), 0.5));

        f32 cont = FLT_MAX;

        if (vert.pos.y > 1.7)
            cont = 0.0f;

        constraints.push_back(PxClothParticleMotionConstraint(core::conv::toPx(vert.pos), cont));
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
    m_cloth->setStiffnessFrequency(20.0f);

    m_cloth->setDampingCoefficient(PxVec3(0.1f));
    m_cloth->setLinearDragCoefficient(PxVec3(0.1f));
    m_cloth->setAngularDragCoefficient(PxVec3(0.1f));

    m_cloth->setMotionConstraints(&constraints[0]);

    // m_cloth->setSelfCollisionDistance(0.01f);
    // m_cloth->setSelfCollisionStiffness(0.5f);

/*    PxClothCollisionSphere spheres[2] =
    {
        PxClothCollisionSphere(PxVec3(0,2,0), 0.1),
        PxClothCollisionSphere(PxVec3(0,0,0), 0.1)
    };

    m_cloth->setCollisionSpheres(spheres, 2);
    m_cloth->addCollisionCapsule(0, 1);*/

    // reduce impact of frame acceleration
    // x, z: cloth swings out less when walking in a circle
    // y: cloth responds less to jump acceleration
    // m_cloth->setLinearInertiaScale(PxVec3(0.1f, 0.1f, 0.1f));
    m_cloth->setInertiaScale(0.01f);

    // leave impact of frame torque at default
    // m_cloth->setAngularInertiaScale(PxVec3(1.0f));

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

void Cloth::drawPoints()
{
    return;
    using namespace physx;
    physx::PxClothParticleData* readData = m_cloth->lockParticleData();

    if (!readData)
        return;

    u32 numVerts = m_cloth->getNbParticles();

    // for (u32 i = 0; i < numVerts; i++)
        // gfx::g_GraphRenderer.addLine(core::conv::toGlm(readData->particles[i].pos), core::conv::toGlm(readData->particles[i].pos) + vec3(0.01), vec3(1.f));

    PxU32 numTriangles = m_meshDesc.triangles.count;
    PxU32 numQuads = m_meshDesc.quads.count;

    PxU16* faces = (PxU16*)malloc(sizeof(PxU16)* (numTriangles*3 + numQuads*6));
    PxU16* fIt = faces;

    PxU8* triangles = (PxU8*)m_meshDesc.triangles.data;
    for (PxU32 i = 0; i < numTriangles; i++)
    {
        if (m_meshDesc.flags & PxMeshFlag::e16_BIT_INDICES)
        {
            PxU16* triangle = (PxU16*)triangles;
            *fIt++ = triangle[ 0 ];
            *fIt++ = triangle[ 1 ];
            *fIt++ = triangle[ 2 ];
        }
        else
        {
            PxU32* triangle = (PxU32*)triangles;
            *fIt++ = triangle[ 0 ];
            *fIt++ = triangle[ 1 ];
            *fIt++ = triangle[ 2 ];
        }
        triangles += m_meshDesc.triangles.stride;
    }

    PxU8* quads = (PxU8*)m_meshDesc.quads.data;
    for (PxU32 i = 0; i < numQuads; i++)
    {
        if (m_meshDesc.flags & PxMeshFlag::e16_BIT_INDICES)
        {
            PxU16* quad = (PxU16*)quads;
            *fIt++ = quad[ 0 ];
            *fIt++ = quad[ 1 ];
            *fIt++ = quad[ 2 ];
            *fIt++ = quad[ 2 ];
            *fIt++ = quad[ 3 ];
            *fIt++ = quad[ 0 ];
        }
        else
        {
            PxU32* quad = (PxU32*)quads;
            *fIt++ = quad[ 0 ];
            *fIt++ = quad[ 1 ];
            *fIt++ = quad[ 2 ];
            *fIt++ = quad[ 2 ];
            *fIt++ = quad[ 3 ];
            *fIt++ = quad[ 0 ];
        }
        quads += m_meshDesc.quads.stride;
    }

    for (auto i = 0; i < (numTriangles + (numQuads * 2)) * 3; i += 3)
    {
        PxVec3 a = m_readData->particles[faces[i]].pos;
        PxVec3 b = m_readData->particles[faces[i + 1]].pos;
        PxVec3 c = m_readData->particles[faces[i + 2]].pos;

        gfx::g_GraphRenderer.addLine(core::conv::toGlm(a), core::conv::toGlm(b), vec3(1.f));
        gfx::g_GraphRenderer.addLine(core::conv::toGlm(b), core::conv::toGlm(c), vec3(1.f));
        gfx::g_GraphRenderer.addLine(core::conv::toGlm(a), core::conv::toGlm(c), vec3(1.f));
    }

    readData->unlock();

    free(faces);
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

u32 Cloth::getTriangleCount()
{
    return m_meshDesc.triangles.count + (m_meshDesc.quads.count * 2);
}

u16* Cloth::getTriangleIndices()
{
    using namespace physx;

    PxU32 numTriangles = m_meshDesc.triangles.count;
    PxU32 numQuads = m_meshDesc.quads.count;

    PxU16* faces = (PxU16*)malloc(sizeof(PxU16)* (numTriangles*3 + numQuads*6));
    PxU16* fIt = faces;

    PxU8* triangles = (PxU8*)m_meshDesc.triangles.data;
    for (PxU32 i = 0; i < numTriangles; i++)
    {
        if (m_meshDesc.flags & PxMeshFlag::e16_BIT_INDICES)
        {
            PxU16* triangle = (PxU16*)triangles;
            *fIt++ = triangle[ 0 ];
            *fIt++ = triangle[ 1 ];
            *fIt++ = triangle[ 2 ];
        }
        else
        {
            PxU32* triangle = (PxU32*)triangles;
            *fIt++ = triangle[ 0 ];
            *fIt++ = triangle[ 1 ];
            *fIt++ = triangle[ 2 ];
        }
        triangles += m_meshDesc.triangles.stride;
    }

    PxU8* quads = (PxU8*)m_meshDesc.quads.data;
    for (PxU32 i = 0; i < numQuads; i++)
    {
        if (m_meshDesc.flags & PxMeshFlag::e16_BIT_INDICES)
        {
            PxU16* quad = (PxU16*)quads;
            *fIt++ = quad[ 0 ];
            *fIt++ = quad[ 1 ];
            *fIt++ = quad[ 2 ];
            *fIt++ = quad[ 2 ];
            *fIt++ = quad[ 3 ];
            *fIt++ = quad[ 0 ];
        }
        else
        {
            PxU32* quad = (PxU32*)quads;
            *fIt++ = quad[ 0 ];
            *fIt++ = quad[ 1 ];
            *fIt++ = quad[ 2 ];
            *fIt++ = quad[ 2 ];
            *fIt++ = quad[ 3 ];
            *fIt++ = quad[ 0 ];
        }
        quads += m_meshDesc.quads.stride;
    }

    return faces;
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
