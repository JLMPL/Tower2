#pragma once
#include "RenderEntity.hpp"
#include "Resource/MeshCache.hpp"

namespace gfx
{
    class Mesh;
}

class RenderMesh : public RenderEntity
{
public:
    using Ptr = std::unique_ptr<RenderMesh>;

    RenderMesh() = default;
    RenderMesh(MeshHandle mesh);
    ~RenderMesh() = default;

    MeshHandle getMesh() const;

    Type getType() const override final;

private:
    MeshHandle m_mesh;
};
