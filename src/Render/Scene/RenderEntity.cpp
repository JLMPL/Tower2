#include "RenderEntity.hpp"

void RenderEntity::setTransform(const mat4& tr)
{
    m_transform = tr;
}

const mat4& RenderEntity::getTransform() const
{
    return m_transform;
}

void RenderEntity::show()
{
    m_visible = true;
}

void RenderEntity::hide()
{
    m_visible = false;
}

bool RenderEntity::isVisible() const
{
    return m_visible;
}
