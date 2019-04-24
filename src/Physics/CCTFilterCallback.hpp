#pragma once
#define _DEBUG
#include <PhysX/PxPhysicsAPI.h>

namespace phys
{

class CCTFilterCallback final : public physx::PxControllerFilterCallback
{
public:
    CCTFilterCallback() = default;
    ~CCTFilterCallback() = default;

    bool filter(const physx::PxController &a, const physx::PxController &b) override final
    {
        return false;
    }
};

}
