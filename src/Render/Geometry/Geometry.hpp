#pragma once
#include "../ArrayBuffer.hpp"
#include "../VertexArray.hpp"
#include "Animation/Animation.hpp"
#include "Resource/MaterialCache.hpp"
#include "Core/Types.hpp"
#include <string>
#include <memory>
#include <vector>

namespace gfx
{

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

        MaterialHandle material;

        VertexArray vao;
        ArrayBuffer dbo;
        ArrayBuffer wbo;
        ArrayBuffer ibo;
    };

    std::string name;

    std::vector<Entry> entries;
};

f32 getVertexHash(const Vertex& vert);

void initScreenQuad();
void renderScreenQuad();

void loadSkinnedMeshFromFile(Mesh& mesh, const Skeleton& skeleton, const std::string& path, bool cloth = false);
void loadStaticMeshFromFile(Mesh& mesh, const std::string& path, bool cloth = false);

}

/*

//things needed to render something
RenderTarget
Vertices
Material
    Textures
*SkinMatrices
Light data

MaterialRenderer
    RenderTargets
    View/Projection
    Shaders

MaterialRenderer* mr = adfsafsd->getMaterialRenderer();
(view, proj) in state

vertexData = {vaos}

void drawVertices(const VertexData& data, const mat4& tr, const mat4* mats = nullptr);
mr->drawVertices(vertexData, modelTransform, skinMatrices)


verts = {
    vec3(pos), vec3(normal), vec2(uv),
    vec3(pos), vec3(normal), vec2(uv),
    vec3(pos), vec3(normal), vec2(uv),
    vec3(pos), vec3(normal), vec2(uv),
    vec3(pos), vec3(normal), vec2(uv)
}

data.setVertexData(verts)
    update vao, dbo, etc.

texture.loadFromFile("data/texture.png")

mat.texture = texture



MaterialRenderer
{
public:
    enum Pass
    {
        SHADOW,
        REFLECTION,
        REFRACTION,
        SOLID
    }

    void setPass(Pass pass)



    void drawVertices(const VertexData& data, const Material& mat, const mat4& tr, const mat4* mats = nullptr)
    {
        switch (pass)
        {
            case SHADOW:
                if (mats)
                {
                    shader = skinnedShader
                }
                else shader = solidShader

                bindvao
                setuniforms m_currentMaterial
            break;
        }
    }

    void finalFrame()

private:
    RenderTarget m_shadow
    RenderTarget m_solid
}

*/