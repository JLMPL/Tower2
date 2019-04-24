#pragma once
#include "../ArrayBuffer.hpp"
#include "../VertexArray.hpp"
#include "Animation/Skeleton.hpp"
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

    void addData(const i32& id, f32 weight)
    {
        for (u32 i = 0; i < 4; i++)
        {
            if (weights[i] == 0.f)
            {
                joints[i] = id;
                weights[i] = weight;
                return;
            }
        }
    }
};

struct SkinVertex
{
    vec3 pos;
    vec3 normal;
    Color color;
    UvCoord uv;
    vec3 tan;
    vec3 bitan;
};

struct SkinnedMesh
{
    struct Entry
    {
        std::vector<SkinVertex>       vertices;
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

}
