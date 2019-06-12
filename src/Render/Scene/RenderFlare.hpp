#pragma once
#include "RenderEntity.hpp"
#include "Render/VertexArray.hpp"
#include "Render/ArrayBuffer.hpp"

namespace gfx
{
    class Texture;
    class BasePass;
}

class RenderFlare : public RenderEntity
{
public:
    using Ptr = std::unique_ptr<RenderFlare>;

    RenderFlare() = default;
    RenderFlare(const gfx::Texture* tex);
    ~RenderFlare() = default;

    void render() const;

    void setColor(const Color& color);
    void setScale(f32 factor);

    void setPosition(const vec3& pos);

    Type getType() const override final;

private:
    gfx::VertexArray m_vao;
    gfx::ArrayBuffer m_vbo;
    gfx::ArrayBuffer m_tbo;

    const gfx::Texture* m_texture = nullptr;

    Color m_color;
    f32 m_scale = 1.f;

    vec3 m_pos;

    friend class gfx::BasePass;
};
