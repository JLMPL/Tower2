#pragma once
#include <Core/Math3D.hpp>
#include <memory>

struct GameEvent;
struct LevelContext;

class Entity
{
public:
    using Ptr = std::unique_ptr<Entity>;

    enum class Type
    {
        Creature = 0,
        Chest,
        Pickup,
        Lever,
        Door,

        LightEffect
    };

    template <typename T>
    T* as()
    {
        return static_cast<T*>(this);
    }

    Entity() = default;
    Entity(u32 id, LevelContext* context);
    virtual ~Entity() = default;

    virtual void       update() = 0;
    virtual void       lateUpdate() {};

    virtual void       onEvent(const GameEvent& event) {}

    virtual Type       getType() const = 0;

    virtual void       setPos(const vec3& pos);
    vec3               getPos() const;

    void               setTransform(const mat4& tr);
    mat4               getTransform() const;

    u32                getID() const;

    virtual void       destroy();
    bool               isDestroyed() const;

protected:
    u32           m_id = 0u;
    LevelContext* m_context = nullptr;

    mat4          m_transform;
    vec3          m_pos;

    bool          m_disabled = false;
    bool          m_destroyed = false;
};
