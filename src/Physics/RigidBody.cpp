#include "RigidBody.hpp"
#include "Core/Convert.hpp"

namespace phys
{

RigidBody::RigidBody(physx::PxRigidDynamic* pxActor) :
    m_pxActor(pxActor)
{
}

bool RigidBody::isValid()
{
    return m_pxActor;
}

void RigidBody::setGlobalTransform(const mat4& tr)
{
    m_pxActor->setGlobalPose(core::conv::toPx(tr));
}

mat4 RigidBody::getGlobalTransform() const
{
    return core::conv::toGlm(m_pxActor->getGlobalPose());
}

vec3 RigidBody::getPosition() const
{
    return getGlobalTransform()[3];
}

void RigidBody::enable()
{
    m_pxActor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);
}

void RigidBody::disable()
{
    m_pxActor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
}

physx::PxRigidDynamic* RigidBody::getPxActor()
{
    return m_pxActor;
}

}
