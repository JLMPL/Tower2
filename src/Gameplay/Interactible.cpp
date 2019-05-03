#include "Interactible.hpp"

Interactible::Interactible(u32 id, LevelContext* context) : Entity(id, context)
{
}

void Interactible::initLabel(const std::string& name, f32 offset)
{
    m_labelName = name;
    m_offset = offset;
}

const std::string& Interactible::getLabelName() const
{
    return m_labelName;
}

f32 Interactible::getLabelOffset() const
{
    return m_offset;
}
