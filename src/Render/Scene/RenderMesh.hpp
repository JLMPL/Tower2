#pragma once
#include "RenderEntity.hpp"

namespace gfx
{
    class Mesh;
}

class RenderMesh : public RenderEntity
{
public:
    using Ptr = std::unique_ptr<RenderMesh>;

    RenderMesh() = default;
    RenderMesh(gfx::Mesh* mesh);
    ~RenderMesh() = default;

    gfx::Mesh* getMesh() const;

    Type getType() const override final;

private:
    gfx::Mesh* m_mesh = nullptr;
};
