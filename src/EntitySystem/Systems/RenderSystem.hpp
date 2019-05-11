#include "EntitySystem/System.hpp"

namespace ecs
{

class RenderSystem : public System
{
public:
    RenderSystem(EntityCollection& ents);

    void forMatchingEntity(u32 ent) override final;
};

}
