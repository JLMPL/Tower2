#pragma once
#include <Core/Math3D.hpp>
#include "EventSystem/EventListener.hpp"
#include <memory>

class Entity : public EventListener
{
public:
    using Ptr = std::unique_ptr<Entity>;

    enum class Type
    {
        Creature = 0,
        Chest,
        Pickup,
        Lever,
        Door
    };

    template <typename T>
    T* as()
    {
        return static_cast<T*>(this);
    }

    Entity() = default;
    Entity(u32 id);
    virtual ~Entity() = default;

    virtual void       update() = 0;
    virtual void       lateUpdate() {};

    virtual Type       getType() const = 0;

    virtual void       setPos(const vec3& pos);
    vec3               getPos() const;

    void               setTransform(const mat4& tr);
    mat4               getTransform() const;

    u32                getId() const;
    const std::string& getCode() const;

    virtual void       disable();
    bool               isDisabled() const;

    void               destroy();
    bool               isDestroyed() const;

protected:
    u32         m_id = 0u;
    std::string m_code = "uncoded";

    mat4        m_transform;
    vec3        m_pos;

    bool        m_disabled = false;
    bool        m_destroyed = false;
};
