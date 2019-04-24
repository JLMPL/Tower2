#pragma once
#include "Core/Math3D.hpp"
#include "Shader.hpp"
#include "Geometry/StaticMesh.hpp"

namespace gfx
{

class Material;
class VertexArray;

struct RenderableInstance
{
    using Ptr = std::unique_ptr<RenderableInstance>;

    static constexpr u32 MaxEntries = 8u;

    mat4               transform;

    const Material*    materials[MaxEntries] = {nullptr};
    const VertexArray* vao[MaxEntries] = {nullptr};
    const mat4*        matrixPalette = nullptr;

    Shader::Type       shaderType = Shader::Type::Static;
    u32                numJoints = 0;

    bool               visible = true;
    bool               nonCullable = false;
    bool               nonShadowCasting = false;
};

class RenderableHandle
{
public:
    RenderableHandle() = default;
    RenderableHandle(RenderableInstance* rend)
        : m_renderable(rend)
    {
    }

    bool isValid()
    {
        return m_renderable;
    }

    void setMesh(const StaticMesh::Entry& entry)
    {
        m_renderable->vao[0] = &entry.vao;
    }

    void setTransform(const mat4& tr)
    {
        m_renderable->transform = tr;
    }

    void show()
    {
        m_renderable->visible = true;
    }

    void hide()
    {
        m_renderable->visible = false;
    }

private:
    RenderableInstance* m_renderable = nullptr;

    friend class Renderer3D;
};

}
