#pragma once
#include "Core/Types.hpp"
#define _DEBUG
#include <PhysX/PxPhysicsAPI.h>

namespace phys
{

class PhysicsAllocator : public physx::PxAllocatorCallback
{
public:
    static constexpr u32 Alignment = 16u;
    PhysicsAllocator() = default;
    ~PhysicsAllocator() = default;

    void* allocate(
        size_t size,
        const char* typeName,
        const char* filename,
        int line) override final
    {
        u64 rawMem = reinterpret_cast<u64>(malloc(size + Alignment));

        u64 mask = Alignment - 1;
        u64 misalignment = (rawMem & mask);
        u64 adjustment = Alignment - misalignment;

        auto memory = reinterpret_cast<byte*>(rawMem + adjustment);
        memory[-1] = adjustment;

        return reinterpret_cast<void*>(memory);
    }

    void deallocate(void* ptr) override final
    {
        if (ptr)
        {
            auto memory = reinterpret_cast<byte*>(ptr);
            byte adjustment = memory[-1];

            uintptr_t aligned = reinterpret_cast<uintptr_t>(memory);
            ptrdiff_t shifted = aligned - adjustment;

            free(reinterpret_cast<void*>(shifted));
        }
    }
};

}
