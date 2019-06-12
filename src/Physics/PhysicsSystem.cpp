#include "PhysicsSystem.hpp"
#include "Debug/Log.hpp"
#include "Debug/DebugMenu.hpp"
#include "Core/Random.hpp"
#include "Render/SceneRenderer.hpp"
#include "Render/MeshManager.hpp"
#include <PhysX/PxRigidActor.h>

namespace phys
{

PhysicsSystem::~PhysicsSystem()
{
    // m_particleSystem->releaseParticles();
    // m_particleSystem->release();
    m_material->release();

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

    m_manager = PxCreateControllerManager(*m_scene);
    m_manager->setOverlapRecoveryModule(true);
    m_manager->setTessellation(true, 0.5);



    /*/
    m_particleSystem = m_physics->createParticleSystem(1000);

    if (!m_particleSystem)
        printf("OUT OF COFFEE!!!\n");

    m_scene->addActor(*m_particleSystem);

    m_particleSystem->setExternalAcceleration(PxVec3(0,9.75f,0));
    m_particleSystem->setDamping(0.1);

    for (u32 i = 0; i < 1000; i++)
    {
        PxParticleCreationData particleCreationData;
        particleCreationData.numParticles = 1;

        PxU32 indices[] = {i};

        PxVec3 poses[] =
        {
            PxVec3(
                core::rand::inRange<f32>(-0.5, 0.5),
                core::rand::inRange<f32>(0,1) + 2.5,
                core::rand::inRange<f32>(-0.5, 0.5)
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

Cloth* PhysicsSystem::addCloth(const std::string& mesh, anim::Animator* animer)
{
    m_cloths.emplace_back(new Cloth(m_physics, mesh, animer));

    m_scene->addActor(*m_cloths.back()->getClothActor());

    return m_cloths.back().get();
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

    for (auto& cloth : m_cloths)
        cloth->skin();

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

        gfx::g_SceneRenderer.addLine(
            vec3(line.pos0.x, line.pos0.y, line.pos0.z),
            vec3(line.pos1.x, line.pos1.y, line.pos1.z),
            vec3(1.f,1.f,1.f)
        );
    }
}

}

