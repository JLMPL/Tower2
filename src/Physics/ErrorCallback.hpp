#pragma once
#include "Core/Types.hpp"
#define _DEBUG
#include <PhysX/PxPhysicsAPI.h>

namespace phys
{

class ErrorCallback : public physx::PxErrorCallback
{
public:
    ErrorCallback() = default;

    void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, i32 line) override final;
};

}
