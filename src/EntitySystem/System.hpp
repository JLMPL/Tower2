#pragma once
#include "Core/Types.hpp"
#include <memory>

namespace ecs
{

struct EntityCollection;

class System
{
    public:
        using Ptr = std::unique_ptr<System>;

        System() = default;
        System(EntityCollection& ents);

        void update();

        virtual void forMatchingEntity(u32 i) = 0;

        bool matchSignature(u32 other);
        bool matchTags(u32 other);

    protected:
        EntityCollection& m_ents;

        u32 m_signature = 0u;
        u32 m_tags = 0u;
};

}
