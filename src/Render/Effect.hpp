#pragma once
#include "Effect.hpp"
#include "ArrayBuffer.hpp"
#include "VertexArray.hpp"

namespace gfx
{

class Effect
{
public:
    Effect() = default;
    virtual ~Effect() = default;

    virtual void show();
    virtual void hide();

protected:
    VertexArray m_vao;
    ArrayBuffer m_vbo;
    ArrayBuffer m_tbo;

    bool m_visible = true;
};

}
