#include "PhysicsSystem.hpp"
#include "Debug/Log.hpp"
#include "Debug/DebugMenu.hpp"
#include "Core/Random.hpp"
#include "Render/GraphRenderer.hpp"
#include "Render/MeshManager.hpp"
#include <PhysX/PxRigidActor.h>

namespace phys
{

PhysicsSystem::~PhysicsSystem()
{
    // m_particleSystem->releaseParticles();
    // m_particleSystem->release();
    m_material->release();
    m_fabric->release();
    m_cloth->release();

    for (auto& i : m_dynamics)
        i->release();

    for (auto& i : m_statics)
        i->release();

    m_manager->release();

    m_scene->release();

    m_cooking->release();
    m_physics->release();
    m_foundation->release();
}

void PhysicsSystem::init()
{
    using namespace physx;

    m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocCallback, m_errorCallback);
    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale());

    PxTolerancesScale scale;
    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, PxCookingParams(scale));

    if (!m_physics)
        Log::error("Could not create PhysX SDK instance!\n");

    PxSceneDesc sceneDesc(m_physics->getTolerancesScale());

    sceneDesc.gravity       = PxVec3(0.f,-9.8f,0.f);
    sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
    sceneDesc.filterShader  = PxDefaultSimulationFilterShader;

    m_scene = m_physics->createScene(sceneDesc);
    m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.f);
    m_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.f);
    m_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_VERTICAL, 1.f);
    m_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_HORIZONTAL, 1.f);
    m_scene->setVisualizationParameter(PxVisualizationParameter::ePARTICLE_SYSTEM_POSITION, 0.1f);

    m_material = m_physics->createMaterial(0.5,0.5,0.5);

    //*/
    {
/*        u32 size = 20;
        u32 size2 = size * size;

        PxClothParticle vertices[size2];

        for (u32 i = 0; i < size; i++)
        for (u32 j = 0; j < size; j++)
        {
            u32 index = i * size + j;
            vertices[index] = PxClothParticle(PxVec3(0, f32(i) * 0.05, f32(j) * 0.05), (i == size-1) ? 0 : 0.1f);
        }

        PxU32 primitives[(size-1)*(size-1)*4];

        u32 primitiveIndex = 0u;

        for (u32 i = 0; i < size-1; i++)
        for (u32 j = 0; j < size-1; j++)
        {
            u32 center = i * size + j;
            u32 right = i * size + j + 1;
            u32 down = (i + 1) * size + j;
            u32 rightDown = (i + 1) * size + j + 1;

            primitives[primitiveIndex++] = rightDown;
            primitives[primitiveIndex++] = right;
            primitives[primitiveIndex++] = center;
            primitives[primitiveIndex++] = down;
        }*/

        auto cape = gfx::g_MeshMgr.getMesh("cape.obj");

        std::vector<PxClothParticle> verts;
        std::vector<PxClothParticleMotionConstraint> constraints;

        for (auto& vert : cape->entries[0].vertices)
        {
            verts.push_back(PxClothParticle(core::conv::toPx(vert.pos), 0.5));

            f32 cont = FLT_MAX;

            if (vert.pos.y > 1.75)
                cont = 0.f;

            constraints.push_back(PxClothParticleMotionConstraint(core::conv::toPx(vert.pos), cont));
        }

        PxClothMeshDesc meshDesc;
        meshDesc.points.data = &verts[0];
        meshDesc.points.count = verts.size();
        meshDesc.points.stride = sizeof(PxClothParticle);

        meshDesc.invMasses.data = &verts[0].invWeight;
        meshDesc.invMasses.count = verts.size();
        meshDesc.invMasses.stride = sizeof(PxClothParticle);

        meshDesc.triangles.data = &cape->entries[0].indices[0];
        meshDesc.triangles.count = cape->entries[0].indices.size()/3;
        meshDesc.triangles.stride = sizeof(u32) * 3;

        if (!meshDesc.isValid())
            printf("NOT VALID!\n");

/*        PxClothMeshDesc meshDesc;
        meshDesc.points.data = vertices;
        meshDesc.points.count = size2;
        meshDesc.points.stride = sizeof(PxClothParticle);

        meshDesc.invMasses.data = &vertices->invWeight;
        meshDesc.invMasses.count = size2;
        meshDesc.invMasses.stride = sizeof(PxClothParticle);

        meshDesc.quads.data = primitives;
        meshDesc.quads.count = (size-1) * (size-1);
        meshDesc.quads.stride = sizeof(PxU32) * 4;*/

        // PxClothMeshQuadifier quadifier(meshDesc);

        // auto quaded = quadifier.getDescriptor();

        m_fabric = PxClothFabricCreate(*m_physics, meshDesc, PxVec3(0, -1, 0));

        // PxTransform pose = PxTransform(PxIdentity);
        PxTransform pose = PxTransform(PxVec3(1,0.5,0));
        m_cloth = m_physics->createCloth(pose, *m_fabric, &verts[0], PxClothFlags());

        m_cloth->setClothFlag(PxClothFlag::eSCENE_COLLISION, false);
        m_cloth->setClothFlag(PxClothFlag::eSWEPT_CONTACT, true);

        m_cloth->setSolverFrequency(240.f);
        m_cloth->setStiffnessFrequency(30.0f);

        m_cloth->setDampingCoefficient(PxVec3(0.5f));
        m_cloth->setLinearDragCoefficient(PxVec3(0.5f));
        m_cloth->setAngularDragCoefficient(PxVec3(0.5f));

        m_cloth->setMotionConstraints(&constraints[0]);


        // m_cloth->setSelfCollisionDistance(0.01f);
        // m_cloth->setSelfCollisionStiffness(0.5f);

        // PxClothCollisionSphere sphere;
        // sphere.pos = PxVec3(2,0,0);
        // sphere.radius = 1.5f;
        // m_cloth->setCollisionSpheres(&sphere, 1);

        PxClothCollisionSphere spheres[2] =
        {
            PxClothCollisionSphere(PxVec3(0,1,0), 0.25f),
            PxClothCollisionSphere(PxVec3(0,0,0), 0.25f)
        };

        m_cloth->setCollisionSpheres(spheres, 2);
        m_cloth->addCollisionCapsule(0, 1);

        // reduce impact of frame acceleration
        // x, z: cloth swings out less when walking in a circle
        // y: cloth responds less to jump acceleration
        // m_cloth->setLinearInertiaScale(PxVec3(0.8f, 0.6f, 0.8f));

        // leave impact of frame torque at default
        // m_cloth->setAngularInertiaScale(PxVec3(1.0f));

        // reduce centrifugal force of rotating frame
        // m_cloth->setCentrifugalInertiaScale(PxVec3(0.3f));

        m_cloth->setStretchConfig(PxClothFabricPhaseType::eVERTICAL, PxClothStretchConfig(1));
        m_cloth->setStretchConfig(PxClothFabricPhaseType::eHORIZONTAL, PxClothStretchConfig(1));

        // PxClothStretchConfig stretchConfig;
        // stretchConfig.stiffness = 0.5f;
        // stretchConfig.stiffnessMultiplier = 0.5f;
        // stretchConfig.compressionLimit = 0.2f;
        // stretchConfig.stretchLimit = 0;
        // m_cloth->setStretchConfig(PxClothFabricPhaseType::eVERTICAL, stretchConfig);
        // m_cloth->setStretchConfig(PxClothFabricPhaseType::eHORIZONTAL, stretchConfig);

        // m_cloth->setSelfCollisionDistance(0.01f);

        m_scene->addActor(*m_cloth);
    }
    //*/

    m_manager = PxCreateControllerManager(*m_scene);
    m_manager->setOverlapRecoveryModule(true);
    m_manager->setTessellation(true, 0.5);


    /*/
    m_particleSystem = m_physics->createParticleSystem(1000);

    if (!m_particleSystem)
        printf("OUT OF COFFEE!!!\n");

    m_scene->addActor(*m_particleSystem);

    m_particleSystem->setExternalAcceleration(PxVec3(0,9.75f,0));
    m_particleSystem->setDamping(1);

    for (u32 i = 0; i < 999; i++)
    {
        PxParticleCreationData particleCreationData;
        particleCreationData.numParticles = 1;

        PxU32 indices[] = {i};

        PxVec3 poses[] =
        {
            PxVec3(
                Random::inRange<f32>(-0.5, 0.5),
                Random::inRange<f32>(0,1) + 2.5,
                Random::inRange<f32>(-0.5, 0.5)
            )
        };

        PxVec3 vels[] =
        {
            PxVec3(1 + i * 0.001, 10, 0)
        };

        particleCreationData.indexBuffer = PxStrideIterator<const PxU32>(indices);
        particleCreationData.positionBuffer = PxStrideIterator<const PxVec3>(poses);
        particleCreationData.velocityBuffer = PxStrideIterator<const PxVec3>(vels);

        bool success = m_particleSystem->createParticles(particleCreationData);

        if (!success)
            printf("OUT OF CREAM!!!\n");
    }
    //*/

    debug::g_Menu["Physics"].bind("debugDraw", &m_debugDraw);
}

physx::PxController* PhysicsSystem::addController(u32* entityID, f32 radius, f32 height)
{
    physx::PxCapsuleControllerDesc desc;

    desc.contactOffset = 0.02;
    desc.position = physx::PxExtendedVec3(0,0,0);
    desc.stepOffset = 0.2;
    desc.slopeLimit = cos(HALF_PI * 0.5);
    desc.radius = radius;
    desc.height = height;
    desc.upDirection = physx::PxVec3(0,1,0);
    desc.material = m_material;
    desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;

    if (!desc.isValid())
        Log::error("PxCapsuleControllerDesc is not valid!\n");

    auto cct = m_manager->createController(desc);

    cct->getActor()->userData = reinterpret_cast<void*>(entityID);

    // physx::PxShape* shape = nullptr;
    // cct->getActor()->getShapes(&shape, 1);
    // shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);

    return cct;
}

void PhysicsSystem::addTriangleMesh(const physx::PxTriangleMeshDesc& desc)
{
    physx::PxDefaultMemoryOutputStream writeBuffer;

    bool status = m_cooking->cookTriangleMesh(desc, writeBuffer);

    if (!status)
        Log::error("PhysicsSystem could not create TriangleMesh!\n");

    physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
    m_triangleMeshes.push_back(m_physics->createTriangleMesh(readBuffer));

    physx::PxTransform tr = physx::PxTransform(physx::PxVec3(0.0f));

    auto actor = m_physics->createRigidStatic(tr);
    actor->createShape(physx::PxTriangleMeshGeometry(m_triangleMeshes.back()), *m_material);

    m_scene->addActor(*actor);
    m_statics.push_back(actor);
}

void PhysicsSystem::addHeightField(const physx::PxHeightFieldDesc& desc, const vec3& scale)
{
    physx::PxHeightField* heightfield =
        m_cooking->createHeightField(desc, m_physics->getPhysicsInsertionCallback());

    physx::PxHeightFieldGeometry geometry(heightfield, physx::PxMeshGeometryFlags(), scale.y, scale.x, scale.z);

    physx::PxTransform tr =
        physx::PxTransform(physx::PxVec3(0.0f));//, physx::PxQuat(-HALF_PI, physx::PxVec3(0,1,0)));
    auto actor = m_physics->createRigidStatic(tr);
    actor->createShape(geometry, *m_material);

    // physx::PxShape* shape = physx::PxRigidActor::createShape(geometry, m_material);

    m_scene->addActor(*actor);
    m_statics.push_back(actor);
}

RigidBody PhysicsSystem::addBox(const vec3& pos, const vec3& vel, const vec3& size)
{
    physx::PxTransform boxPos(physx::PxVec3(pos.x, pos.y, pos.z), physx::PxQuat(physx::PxHalfPi, physx::PxVec3(1.f,0.f,1.f)));

    physx::PxBoxGeometry boxGeometry(core::conv::toPx(size));
    auto box = PxCreateDynamic(*m_physics, boxPos, boxGeometry, *m_material, 0.5f);

    m_scene->addActor(*box);

    box->setLinearVelocity(physx::PxVec3(vel.x, vel.y, vel.z));
    box->setAngularVelocity(physx::PxVec3(11,11,11));

    return RigidBody(box);
}

RigidBody PhysicsSystem::addCapsule(const vec3& pos, const vec3& vel, f32 radius, f32 halfHeight)
{
    physx::PxTransform cpos(physx::PxVec3(pos.x, pos.y, pos.z), physx::PxQuat(physx::PxHalfPi, physx::PxVec3(1.f,0.f,0.f)));

    physx::PxCapsuleGeometry geom(radius, halfHeight);
    auto cap = PxCreateDynamic(*m_physics, cpos, geom, *m_material, 0.5f);

    m_scene->addActor(*cap);

    // cap->setLinearVelocity(physx::PxVec3(vel.x, vel.y, vel.z));

    return RigidBody(cap);
}

StaticBody PhysicsSystem::addStaticBox(const vec3& size, const vec3& pos, const quat& rot)
{
    physx::PxTransform tr(core::conv::toPx(pos), core::conv::toPx(rot));
    physx::PxBoxGeometry boxGeometry(core::conv::toPx(size));

    auto actor = m_physics->createRigidStatic(tr);
    actor->createShape(boxGeometry, *m_material);

    m_scene->addActor(*actor);
    m_statics.push_back(actor);

    return StaticBody(actor);
}

void PhysicsSystem::removeRigidBody(RigidBody& rb)
{
    m_scene->removeActor(*rb.getPxActor());
}

RaycastResult PhysicsSystem::raycast(const vec3& origin, const vec3& dir, f32 dist)
{
    physx::PxRaycastBuffer rb;
    RaycastResult rr;

    rr.hasHit = m_scene->raycast(core::conv::toPx(origin), core::conv::toPx(dir), dist, rb);

    if (rr.hasHit)
    {
        if (rb.block.actor->userData)
            rr.entityID = *(reinterpret_cast<u32*>(rb.block.actor->userData));
    }

    rr.pos = core::conv::toGlm(rb.block.position);
    rr.normal = core::conv::toGlm(rb.block.normal);

    return rr;
}

SweepResult PhysicsSystem::sweepSphere(f32 radius, const vec3& origin, const vec3& dir, f32 dist)
{
    physx::PxSweepBuffer hit;
    SweepResult sr;

    physx::PxTransform();
    sr.hasHit = m_scene->sweep(
        physx::PxSphereGeometry(radius),
        physx::PxTransform(core::conv::toPx(origin)),
        core::conv::toPx(dir),
        dist, hit,
        physx::PxHitFlag::eDEFAULT,
        physx::PxQueryFilterData(physx::PxQueryFlag::eSTATIC)
    );

    sr.pos = core::conv::toGlm(hit.block.position);
    sr.normal = core::conv::toGlm(hit.block.normal);

    return sr;
}

void PhysicsSystem::preSimulationUpdate()
{
    m_manager->computeInteractions(core::g_FInfo.delta, &m_cctFilterCallback);
}

void PhysicsSystem::stepSimulation()
{
    using namespace physx;

    m_sinning += core::g_FInfo.delta;
    m_cloth->setExternalAcceleration(physx::PxVec3(0,0,sin(m_sinning * 2) * 10));

    mat4 clothTr = math::translate(vec3(-0.5,2,sin(m_sinning)*2)) * math::rotate(f32(HALF_PI), vec3(0,1,0));

    mat4 invCloth = math::inverse(clothTr);

    // PxClothCollisionSphere spheres[2] =
    // {
    //     PxClothCollisionSphere(core::conv::toPx(vec3(vec4(-2,0,0,1) * invCloth)), 0.5),
    //     PxClothCollisionSphere(core::conv::toPx(vec3(vec4(0,0,0,1) * invCloth)), 0.5)
    // };

    // m_cloth->setCollisionSpheres(spheres, 2);

    // m_cloth->setTargetPose(core::conv::toPx(math::translate(vec3(1,1,0))));

    m_scene->simulate(core::g_FInfo.delta);
    m_scene->fetchResults(true);
}

void PhysicsSystem::debugDraw()
{
    if (!m_debugDraw)
        return;

    using namespace physx;

    const PxRenderBuffer& buff = m_scene->getRenderBuffer();

    for(uint32_t i = 0; i < buff.getNbLines(); i++)
    {
        const PxDebugLine& line = buff.getLines()[i];

        gfx::g_GraphRenderer.addLine(
            vec3(line.pos0.x, line.pos0.y, line.pos0.z),
            vec3(line.pos1.x, line.pos1.y, line.pos1.z),
            vec3(1.f,1.f,1.f)
        );
    }
}

}

