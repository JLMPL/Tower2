#include "TransformSystem.hpp"
#include "EntitySystem/Components.hpp"

namespace ecs
{

TransformSystem::TransformSystem(EntityCollection& ents) :
    System(ents)
{
    m_signature = comp::Transform::Signature;
}

void TransformSystem::forMatchingEntity(u32 ent)
{
    // printf("transforming the transform like transformers\n");
}

}
