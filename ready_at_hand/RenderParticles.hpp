#pragma once
#include "RenderEntity.hpp"
#include "Render/VertexArray.hpp"
#include "Render/ArrayBuffer.hpp"

class ParticleGroup;

namespace gfx
{

class Texture;
class BasePass;

}

class RenderParticles : public RenderEntity
{
public:
    using Ptr = std::unique_ptr<RenderParticles>;

    RenderParticles() = default;
    RenderParticles(const ParticleGroup* group);
    ~RenderParticles();

    void updateTransforms();
    void render();

    u32 getParticleCount();
    mat4* getMatrixArray();

    Type getType() const override final;

private:
    const ParticleGroup* m_group = nullptr;

    gfx::VertexArray m_vao;
    gfx::ArrayBuffer m_vbo;
    gfx::ArrayBuffer m_tbo;

    const gfx::Texture* m_texture = nullptr;
    u32 m_numParticles = 0;

    mat4* m_transforms = nullptr;

    Color m_color;
    f32 m_scale = 1.f;

    vec3 m_pos;

    friend class gfx::BasePass;
};
