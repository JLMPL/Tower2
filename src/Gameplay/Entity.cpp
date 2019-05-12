#include "Entity.hpp"

Entity::Entity(u32 id, LevelContext* context) :
    m_id(id), m_context(context)
{
}

void Entity::setPos(const vec3& pos)
{
    m_pos = pos;
}

vec3 Entity::getPos() const
{
    return m_pos;
}

void Entity::setTransform(const mat4& tr)
{
    m_transform = tr;
}

mat4 Entity::getTransform() const
{
    return m_transform;
}

u32 Entity::getID() const
{
    return m_id;
}

void Entity::disable()
{
    m_disabled = true;
}

bool Entity::isDisabled() const
{
    return m_disabled;
}

void Entity::destroy()
{
    m_destroyed = true;
}

bool Entity::isDestroyed() const
{
    return m_destroyed;
}

