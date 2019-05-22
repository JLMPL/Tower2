#include "StaticBody.hpp"
#include "Core/Convert.hpp"

namespace phys
{

StaticBody::StaticBody(physx::PxRigidStatic* actor) :
    m_pxActor(actor)
{
}

bool StaticBody::isValid()
{
    return m_pxActor;
}

void StaticBody::setGlobalTransform(const mat4& tr)
{
    m_pxActor->setGlobalPose(core::conv::toPx(tr));
}

mat4 StaticBody::getGlobalTransform() const
{
    return core::conv::toGlm(m_pxActor->getGlobalPose());
}

vec3 StaticBody::getPosition() const
{
    return getGlobalTransform()[3];
}

physx::PxRigidStatic* StaticBody::getPxActor()
{
    return m_pxActor;
}

}
