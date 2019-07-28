#include "Geometry.hpp"
#include "Core/Types.hpp"

namespace gfx
{

LOCAL VertexArray vao;
LOCAL ArrayBuffer vbo;
LOCAL ArrayBuffer tbo;

void initScreenQuad()
{
    constexpr f32 verts[] = {
        1,-1,
        1,1,
        -1,1,
        1,-1,
        -1,1,
        -1,-1
    };

    constexpr f32 coords[] = {
        1,0,
        1,1,
        0,1,
        1,0,
        0,1,
        0,0
    };

    createVertexArray(vao);
    bindVertexArray(vao);

    vbo.init(GL_ARRAY_BUFFER);
    vbo.bind();
    vbo.setData(sizeof(f32) * 12, &verts[0], GL_STATIC_DRAW);
    setVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    tbo.init(GL_ARRAY_BUFFER);
    tbo.bind();
    tbo.setData(sizeof(f32) * 12, &coords[0], GL_STATIC_DRAW);
    setVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    unbindVertexArray(vao);
    vbo.unbind();
}

void renderScreenQuad()
{
    bindVertexArray(vao);
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    unbindVertexArray(vao);
}

f32 getVertexHash(const Vertex& vert)
{
    return vert.pos.x + vert.pos.y + vert.pos.z +
        vert.normal.x + vert.normal.y + vert.normal.z +
        vert.uv.x + vert.uv.y;
}

}
