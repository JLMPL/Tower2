#pragma once
#include "Core/Math3D.hpp"
#define _DEBUG
#include <PhysX/PxPhysicsAPI.h>

namespace phys
{

class StaticBody
{
    public:
        StaticBody() = default;
        StaticBody(physx::PxRigidStatic* actor);

        bool isValid();

        void setGlobalTransform(const mat4& tr);
        mat4 getGlobalTransform() const;
        vec3 getPosition() const;

        physx::PxRigidStatic* getPxActor();

    private:
        physx::PxRigidStatic* m_pxActor = nullptr;
};

}
