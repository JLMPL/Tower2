#include "System.hpp"
#include "Scene.hpp"

namespace ecs
{

System::System(EntityCollection& ents) :
    m_ents(ents)
{
}

void System::update()
{
    for (u32 i = 0; i < EntityCollection::MaxEntities; i++)
    {
        if (matchSignature(m_ents.masks[i]) && (matchTags(m_ents.tags[i]) || m_tags == 0))
        {
            forMatchingEntity(i);
        }
    }
}

bool System::matchSignature(u32 other)
{
    return (m_signature & other) == m_signature;
}

bool System::matchTags(u32 other)
{
    return (m_tags & other) == m_tags;
}

}
