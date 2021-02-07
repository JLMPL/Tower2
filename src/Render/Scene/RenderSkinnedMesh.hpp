#pragma once
#include "RenderEntity.hpp"
#include "Resource/MeshCache.hpp"

namespace gfx
{
    class Mesh;
}

class RenderSkinnedMesh : public RenderEntity
{
public:
    using Ptr = std::unique_ptr<RenderSkinnedMesh>;

    RenderSkinnedMesh() = default;
    RenderSkinnedMesh(MeshHandle mesh, const mat4* palette);
    ~RenderSkinnedMesh() = default;

    MeshHandle getMesh() const;

    i32 getNumJoints() const;
    const mat4* getMatrixPalette() const;

    Type getType() const override final;

private:
    MeshHandle m_mesh;
    const mat4* m_matrixPalette = nullptr;
};
