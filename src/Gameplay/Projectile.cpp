#include "Projectile.hpp"

void Projectile::destroy()
{
    m_destroyed = true;
}

bool Projectile::isDestroyed() const
{
    return m_destroyed;
}
