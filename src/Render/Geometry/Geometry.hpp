#pragma once
#include "../ArrayBuffer.hpp"
#include "../VertexArray.hpp"
#include "Animation/Animation.hpp"
#include "Core/Types.hpp"
#include <string>
#include <memory>
#include <vector>

namespace gfx
{

class Material;

struct VertexWeightData
{
    i32 joints[4] = {0};
    f32 weights[4] = {0.f};
};

struct Vertex
{
    vec3 pos;
    vec3 normal;
    Color color;
    UvCoord uv;
    vec3 tan;
    vec3 bitan;
};

struct Mesh
{
    struct Entry
    {
        std::vector<Vertex>           vertices;
        std::vector<VertexWeightData> weightsData;
        std::vector<u32>              indices;

        const Material* material = nullptr;

        VertexArray vao;
        ArrayBuffer dbo;
        ArrayBuffer wbo;
        ArrayBuffer ibo;
    };

    std::string name;

    std::vector<Entry> entries;
    anim::Skeleton skeleton;
};

f32 getVertexHash(const Vertex& vert);

void initScreenQuad();
void renderScreenQuad();

void loadSkinnedMeshFromFile(Mesh& mesh, const std::string& path, bool cloth = false);
void loadStaticMeshFromFile(Mesh& mesh, const std::string& path, bool cloth = false);

}
