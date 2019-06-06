#pragma once
#include "SceneNode.hpp"
#include "Render/Geometry/StaticMesh.hpp"
#include "Render/VertexArray.hpp"
#include "Render/ArrayBuffer.hpp"

namespace phys
{
    class Cloth;
}

namespace gfx
{
    class BasePass;
}

class ClothNode : public SceneNode
{
public:
    ClothNode() = default;
    ClothNode(phys::Cloth* cloth);
    ~ClothNode() = default;

    void updateBuffers();
    void render() const;

    Type getType() const override final;

private:
    phys::Cloth* m_cloth = nullptr;
    gfx::StaticMesh* m_mesh = nullptr;

    std::vector<gfx::Vertex> m_vertices;

    gfx::VertexArray m_vao;
    gfx::ArrayBuffer m_dbo;
    gfx::ArrayBuffer m_ibo;

    friend class gfx::BasePass;
};
