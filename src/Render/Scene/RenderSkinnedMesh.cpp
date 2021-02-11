#include "RenderSkinnedMesh.hpp"
#include "Animation/Animation.hpp"
#include "Render/Geometry/Geometry.hpp"

RenderSkinnedMesh::RenderSkinnedMesh(MeshHandle mm, const std::vector<mat4>& palette) :
    mesh(mm), matrixPalette(palette)
{
}
