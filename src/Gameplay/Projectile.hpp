#pragma once
#include <Core/Math3D.hpp>
#include <memory>

class Projectile
{
public:
    using Ptr = std::unique_ptr<Projectile>;

    enum Type
    {
        Arrow = 0,
        Fireball,

        Count
    };

    virtual ~Projectile() = default;

    virtual void init(const vec3& pos, const vec3& dir, f32 speed) = 0;
    virtual void update() = 0;
    virtual Type getType() const = 0;

    void destroy();
    bool isDestroyed() const;

protected:
    mat4 m_transform;
    vec3 m_pos;
    vec3 m_dir = vec3(1,0,0);

    f32 m_speed = 1.f;

    bool m_destroyed = false;
};
