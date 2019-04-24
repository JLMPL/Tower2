#include "Entity.hpp"

void Entity::init(u32 id, const std::string& code)
{
    m_id = id;
    m_code = code;
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

u32 Entity::getId() const
{
    return m_id;
}

const std::string& Entity::getCode() const
{
    return m_code;
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

