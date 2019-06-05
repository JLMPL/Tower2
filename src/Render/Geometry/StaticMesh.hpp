#pragma once
#include "../OpenGL.hpp"
#include "../VertexArray.hpp"
#include "../ArrayBuffer.hpp"
#include "Core/Math3D.hpp"
#include <memory>
#include <vector>

namespace gfx
{

struct Material;

struct Vertex
{
    vec3    pos;
    vec3    normal;
    Color   color;
    UvCoord uv;
    vec3    tan;
    vec3    bitan;

    f32 getHash()
    {
        return pos.x + pos.y + pos.z +
            normal.x + normal.y + normal.z +
            uv.x + uv.y;
    }
};

struct StaticMesh
{
    struct Triangle
    {
        u32 verts[3] = {0,0,0};
    };

    struct Entry
    {
        std::vector<Vertex>  vertices;
        // std::vector<Triangle> indices;
        std::vector<u32> indices;

        const Material* material = nullptr;

        VertexArray vao;
        ArrayBuffer dbo;
        ArrayBuffer ibo;
    };

    std::string name;
    std::vector<Entry> entries;
};

}
