#pragma once
#include "Components.hpp"
#include "System.hpp"
#include <vector>

namespace ecs
{

static constexpr i32 InvalidEntityId = -1;

struct EntityHandle
{
    i32 id = -1;
    i32 generation = -1;
};

struct EntityCollection
{
    static constexpr u32 MaxEntities = 1024u;

    u32 masks[MaxEntities];
    u32 generations[MaxEntities];
    u32 tags[MaxEntities];

    ecs::comp::Transform transforms[MaxEntities];
    ecs::comp::StaticMesh staticMeshes[MaxEntities];
};

class Scene
{
public:
    Scene();
    ~Scene() = default;

    void initSystems();

    void update();

    EntityHandle createEntity();
    void destroyEntity(EntityHandle& entity);

    template <typename T>
    T* addComponent(const EntityHandle& entity)
    {
        if (m_ents.generations[entity.id] != entity.generation)
            return nullptr;

        m_ents.masks[entity.id] |= T::Signature;

        return getComponent<T>(entity);
    }

    template <typename T>
    constexpr T* getComponent(const EntityHandle& entity)
    {
        // if (m_ents.generations[entity.id] != entity.generation)
        //     return nullptr;

        if constexpr (T::Signature == comp::Transform::Signature)
        {
            return &m_ents.transforms[entity.id];
        }
        else if constexpr (T::Signature == comp::StaticMesh::Signature)
        {
            return &m_ents.staticMeshes[entity.id];
        }

        return nullptr;
    }

    EntityCollection& getEntityCollection();

private:
    std::vector<System::Ptr> m_systems;
    EntityCollection m_ents;
};

}
