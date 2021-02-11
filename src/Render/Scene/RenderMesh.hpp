#pragma once
#include "RenderEntity.hpp"
#include "Resource/MeshCache.hpp"

struct RenderMesh : public RenderEntity
{
    using Ptr = std::unique_ptr<RenderMesh>;
    using Ref = std::shared_ptr<RenderMesh>;

    RenderMesh() = default;
    RenderMesh(MeshHandle mesh);
    ~RenderMesh() = default;

    MeshHandle mesh;
};
