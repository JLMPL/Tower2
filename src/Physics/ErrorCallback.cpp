#include "ErrorCallback.hpp"
#include "Debug/Log.hpp"

namespace phys
{

void ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, i32 line)
{
    switch (code)
    {
        case physx::PxErrorCode::eDEBUG_INFO:
            Log::info("%s in %s:%d\n", message, file, line);
            break;

        case physx::PxErrorCode::eDEBUG_WARNING:
            Log::warning("%s in %s:%d\n", message, file, line);
            break;

        default: break;
    }
}

}
