#include "EntitySystem/System.hpp"

namespace ecs
{

class TransformSystem : public System
{
public:
    TransformSystem(EntityCollection& ents);

    void forMatchingEntity(u32 ent) override final;
};

}
