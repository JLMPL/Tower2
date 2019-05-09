#include "Scene.hpp"
#include "Systems/TransformSystem.hpp"

namespace ecs
{

Scene::Scene()
{
    memset(m_ents.masks, 0, sizeof(u32) * EntityCollection::MaxEntities);
    memset(m_ents.generations, 0, sizeof(u32) * EntityCollection::MaxEntities);
    memset(m_ents.tags, 0, sizeof(u32) * EntityCollection::MaxEntities);
}

void Scene::initSystems()
{
    m_systems.emplace_back(new TransformSystem(m_ents));
}

void Scene::update()
{
    for (auto& sys : m_systems)
        sys->update();
}

EntityHandle Scene::createEntity()
{
    for (u32 i = 0; i < EntityCollection::MaxEntities; i++)
    {
        if (m_ents.masks[i] == 0)
            return {i, m_ents.generations[i]};
    }

    return {-1,-1};
}

void Scene::destroyEntity(EntityHandle& entity)
{
    if (entity.generation == m_ents.generations[entity.id])
    {
        m_ents.masks[entity.id] = 0;
        m_ents.generations[entity.id]++;

        entity.id = -1;
        entity.generation = -1;
    }
}

EntityCollection& Scene::getEntityCollection()
{
    return m_ents;
}

}
