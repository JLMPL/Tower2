#include "RenderParticles.hpp"
#include "Render/Effects/ParticleGroup.hpp"

RenderParticles::RenderParticles(const ParticleGroup* group) :
    m_group(group)
{
    m_numParticles = m_group->m_numParticles;

    m_transforms = (mat4*)malloc(sizeof(mat4) * m_numParticles);

    constexpr f32 verts[] = {
        0.5,-0.5,
        0.5,0.5,
        -0.5,0.5,
        0.5,-0.5,
        -0.5,0.5,
        -0.5,-0.5
    };

    constexpr f32 coords[] = {
        1,0,
        1,1,
        0,1,
        1,0,
        0,1,
        0,0
    };

    gfx::createVertexArray(m_vao);
    gfx::bindVertexArray(m_vao);

    m_vbo.init(GL_ARRAY_BUFFER);
    m_vbo.bind();
    m_vbo.setData(sizeof(f32) * 12, &verts[0], GL_STATIC_DRAW);
    gfx::setVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    m_tbo.init(GL_ARRAY_BUFFER);
    m_tbo.bind();
    m_tbo.setData(sizeof(f32) * 12, &coords[0], GL_STATIC_DRAW);
    gfx::setVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    gfx::unbindVertexArray(m_vao);
    m_vbo.unbind();
}

RenderParticles::~RenderParticles()
{
    free(m_transforms);
}

void RenderParticles::updateTransforms()
{
    for (auto i = 0; i < getParticleCount(); i++)
    {
        m_transforms[i] = math::translate(m_group->m_particles[i].pos);
    }
}

void RenderParticles::render()
{
    bindVertexArray(m_vao);
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    unbindVertexArray(m_vao);
}

u32 RenderParticles::getParticleCount()
{
    return m_numParticles;
}

mat4* RenderParticles::getMatrixArray()
{
    return m_transforms;
}

RenderEntity::Type RenderParticles::getType() const
{
    return Type::Particles;
}

