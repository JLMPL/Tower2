#include "RenderMesh.hpp"
#include "Animation/Animation.hpp"
#include "Render/Geometry/Geometry.hpp"

RenderMesh::RenderMesh(MeshHandle mesh) :
    m_mesh(mesh)
{
}

MeshHandle RenderMesh::getMesh() const
{
    return m_mesh;
}

RenderEntity::Type RenderMesh::getType() const
{
    return Type::Mesh;
}
