#pragma once
#include "Core/Math3D.hpp"
#define _DEBUG
#include <PhysX/PxPhysicsAPI.h>

namespace phys
{

class RigidBody
{
    public:
        RigidBody() = default;
        RigidBody(physx::PxRigidDynamic* pxActor);

        bool isValid();

        void setGlobalTransform(const mat4& tr);
        mat4 getGlobalTransform() const;
        vec3 getPosition() const;

        void enable();
        void disable();

        physx::PxRigidDynamic* getPxActor();

    private:
        physx::PxRigidDynamic* m_pxActor = nullptr;
};

}
