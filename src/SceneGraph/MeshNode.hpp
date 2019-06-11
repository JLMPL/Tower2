#pragma once
#include "SceneNode.hpp"

namespace gfx
{
    class Mesh;
}

class MeshNode : public SceneNode
{
public:
    MeshNode() = default;
    MeshNode(gfx::Mesh* mesh);
    ~MeshNode() = default;

    void setMesh(gfx::Mesh* mesh);
    gfx::Mesh* getMesh() const;

    Type getType() const override final;

private:
    gfx::Mesh* m_mesh = nullptr;
};
