#include "MeshNode.hpp"

MeshNode::MeshNode(gfx::StaticMesh* mesh) :
    m_mesh(mesh)
{
}

void MeshNode::setMesh(gfx::StaticMesh* mesh)
{
    m_mesh = mesh;
}

gfx::StaticMesh* MeshNode::getMesh() const
{
    return m_mesh;
}

SceneNode::Type MeshNode::getType() const
{
    return Type::Mesh;
}
