#pragma once
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"

namespace gfx
{

struct ScreenQuad
{
    VertexArray vao;
    ArrayBuffer vbo;
    ArrayBuffer tbo;

    void init();
    void render() const;
};

}
