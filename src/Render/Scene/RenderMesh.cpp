#include "RenderMesh.hpp"
#include "Animation/AnimationSystem.hpp"
#include "Render/Geometry/Mesh.hpp"

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
