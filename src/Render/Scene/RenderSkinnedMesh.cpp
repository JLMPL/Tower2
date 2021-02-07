#include "RenderSkinnedMesh.hpp"
#include "Animation/Animation.hpp"
#include "Render/Geometry/Geometry.hpp"

RenderSkinnedMesh::RenderSkinnedMesh(MeshHandle mesh, const mat4* palette) :
    m_mesh(mesh), m_matrixPalette(palette)
{
}

MeshHandle RenderSkinnedMesh::getMesh() const
{
    return m_mesh;
}

i32 RenderSkinnedMesh::getNumJoints() const
{
    return m_mesh->skeleton.joints.size();
}

const mat4* RenderSkinnedMesh::getMatrixPalette() const
{
    return m_matrixPalette;
}

RenderEntity::Type RenderSkinnedMesh::getType() const
{
    return Type::SkinMesh;
}
