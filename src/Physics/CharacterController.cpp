#include "CharacterController.hpp"
#include "Core/Convert.hpp"
#include "Core/Math3D.hpp"

namespace phys
{

CharacterController::~CharacterController()
{
    if (m_controller)
        m_controller->release();
}

void CharacterController::init(PhysicsSystem* physSys, u32* entityID, f32 radius, f32 height)
{
    m_physSys = physSys;
    m_radius = radius;
    m_height = height;

    m_controller = m_physSys->addController(entityID, radius, height);
    m_controller->setNonWalkableMode(physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING);//_AND_FORCE_SLIDING);
}

void CharacterController::move(const vec3& move, f32 deltaTime, bool followGround)
{
    physx::PxControllerFilters filters;
    filters.mCCTFilterCallback = m_physSys->getCCTFilter();
    vec3 nuo = move * deltaTime;
    nuo.y -= 0.02f;

    m_controller->move(core::conv::toPx(nuo), 0.001f, deltaTime, filters);

    auto hit = m_physSys->sweepSphere(
        m_radius,
        core::conv::toGlm(m_controller->getPosition()),
        vec3(0,-1,0),
        (m_height * 0.5) + 0.1
    );

    m_onGround = hit.hasHit;
    m_groundNormal = hit.normal;
}

bool CharacterController::isOnGround() const
{
    return m_onGround;
}

vec3 CharacterController::getGroundNormal() const
{
    return m_groundNormal;
}

void CharacterController::setPosition(const vec3& pos)
{
    m_controller->setPosition(physx::PxExtendedVec3(pos.x, pos.y, pos.z));
}

vec3 CharacterController::getPosition() const
{
    return core::conv::toGlm(m_controller->getPosition());
}

vec3 CharacterController::getFootPosition() const
{
    return core::conv::toGlm(m_controller->getFootPosition());
}

}
