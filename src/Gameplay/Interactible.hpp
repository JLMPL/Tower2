#pragma once
#include "Entity.hpp"

class Interactible : public Entity
{
public:
    virtual ~Interactible() = default;

    void initLabel(const std::string& name, f32 offset);

    virtual void interact(class Creature* other) = 0;

    const std::string& getLabelName() const;
    f32 getLabelOffset() const;

protected:
    mat4         m_interactTransform;
    bool         m_requiresTransform = false;

    std::string  m_labelName = "[unnamed]";
    f32          m_offset = 1.f;
};
