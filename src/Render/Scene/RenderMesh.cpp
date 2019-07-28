#include "RenderMesh.hpp"
#include "Animation/Animation.hpp"
#include "Render/Geometry/Geometry.hpp"

RenderMesh::RenderMesh(gfx::Mesh* mesh) :
    m_mesh(mesh)
{
}

gfx::Mesh* RenderMesh::getMesh() const
{
    return m_mesh;
}

RenderEntity::Type RenderMesh::getType() const
{
    return Type::Mesh;
}
