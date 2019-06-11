#include "MeshNode.hpp"

MeshNode::MeshNode(gfx::Mesh* mesh) :
    m_mesh(mesh)
{
}

void MeshNode::setMesh(gfx::Mesh* mesh)
{
    m_mesh = mesh;
}

gfx::Mesh* MeshNode::getMesh() const
{
    return m_mesh;
}

SceneNode::Type MeshNode::getType() const
{
    return Type::Mesh;
}
