#pragma once
#include "RenderEntity.hpp"
#include "Resource/MeshCache.hpp"

struct RenderSkinnedMesh : public RenderEntity
{
    using Ptr = std::unique_ptr<RenderSkinnedMesh>;
    using Ref = std::shared_ptr<RenderSkinnedMesh>;

    RenderSkinnedMesh() = default;
    RenderSkinnedMesh(MeshHandle mesh, const std::vector<mat4>& palette);
    ~RenderSkinnedMesh() = default;

    MeshHandle mesh;
    const std::vector<mat4>& matrixPalette;
};
