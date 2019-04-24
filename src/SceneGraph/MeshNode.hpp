#pragma once
#include "SceneNode.hpp"

namespace gfx
{
    class StaticMesh;
}

class MeshNode : public SceneNode
{
public:
    MeshNode() = default;
    MeshNode(gfx::StaticMesh* mesh);
    ~MeshNode() = default;

    void setMesh(gfx::StaticMesh* mesh);
    gfx::StaticMesh* getMesh() const;

    Type getType() const override final;

private:
    gfx::StaticMesh* m_mesh = nullptr;
};
