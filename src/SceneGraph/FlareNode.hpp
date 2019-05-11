#pragma once
#include "SceneNode.hpp"
#include "Render/VertexArray.hpp"
#include "Render/ArrayBuffer.hpp"

namespace gfx
{
    class Texture;
    class BasePass;
}

class FlareNode : public SceneNode
{
public:
    FlareNode() = default;
    FlareNode(const gfx::Texture* tex);
    ~FlareNode() = default;

    void render() const;

    void setColor(const Color& color);
    void setScale(f32 factor);

    Type getType() const override final;

private:
    gfx::VertexArray m_vao;
    gfx::ArrayBuffer m_vbo;
    gfx::ArrayBuffer m_tbo;

    const gfx::Texture* m_texture = nullptr;

    Color m_color;
    f32 m_scale = 1.f;

    friend class gfx::BasePass;
};
