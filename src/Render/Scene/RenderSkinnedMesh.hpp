#pragma once
#include "RenderEntity.hpp"

namespace gfx
{
    class Mesh;
}

class RenderSkinnedMesh : public RenderEntity
{
public:
    using Ptr = std::unique_ptr<RenderSkinnedMesh>;

    RenderSkinnedMesh() = default;
    RenderSkinnedMesh(gfx::Mesh* mesh, const mat4* palette);
    ~RenderSkinnedMesh() = default;

    gfx::Mesh* getMesh() const;

    i32 getNumJoints() const;
    const mat4* getMatrixPalette() const;

    Type getType() const override final;

private:
    gfx::Mesh* m_mesh = nullptr;
    const mat4* m_matrixPalette = nullptr;
};
