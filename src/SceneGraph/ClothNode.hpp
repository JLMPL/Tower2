#pragma once
#include "SceneNode.hpp"
#include "Render/Geometry/StaticMesh.hpp"
#include "Render/Geometry/SkinnedMesh.hpp"
#include "Render/VertexArray.hpp"
#include "Render/ArrayBuffer.hpp"

namespace phys
{
    class Cloth;
}

namespace gfx
{
    class BasePass;
    class Material;
}

class ClothNode : public SceneNode
{
public:
    ClothNode() = default;
    ClothNode(phys::Cloth* cloth);
    ~ClothNode() = default;

    void updateGeometry();
    void render() const;

    const gfx::Material* getMaterial() const;

    Type getType() const override final;

private:
    void updatePositions();
    void updateNormals();
    void updateBuffers();

private:
    phys::Cloth* m_cloth = nullptr;
    gfx::SkinnedMesh* m_mesh = nullptr;

    std::vector<gfx::SkinVertex> m_vertices;

    gfx::VertexArray m_vao;
    gfx::ArrayBuffer m_dbo;
    gfx::ArrayBuffer m_ibo;

    friend class gfx::BasePass;
};
