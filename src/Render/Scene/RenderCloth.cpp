#include "RenderCloth.hpp"
#include "Render/Geometry/Geometry.hpp"
#include "Physics/Cloth.hpp"

RenderCloth::RenderCloth(phys::Cloth* cloth) :
    m_cloth(cloth)
{
    m_mesh = m_cloth->getMesh();

    m_vertices.resize(m_mesh->entries[0].vertices.size());

    for (auto i = 0; i < m_vertices.size(); i++)
    {
        m_vertices[i] = m_mesh->entries[0].vertices[i];
    }
}

void RenderCloth::updateGeometry()
{
    updatePositions();
    updateNormals();
    updateBuffers();
}

void RenderCloth::updatePositions()
{
    for (auto i = 0; i < m_cloth->getVertexCount(); i++)
    {
        m_vertices[i].pos = m_cloth->getVertices()[i];
    }
}

void RenderCloth::updateNormals()
{
    for (auto& vert : m_vertices)
        vert.normal = vec3(0);

    for (auto i = 0; i < m_mesh->entries[0].indices.size(); i += 3)
    {
        i32 index0 = m_mesh->entries[0].indices[i];
        i32 index1 = m_mesh->entries[0].indices[i + 1];
        i32 index2 = m_mesh->entries[0].indices[i + 2];

        vec3 a = m_vertices[index0].pos;
        vec3 b = m_vertices[index1].pos;
        vec3 c = m_vertices[index2].pos;

        vec3 n = math::normalize(math::cross(b - a, c - a));

        m_vertices[index0].normal += n;
        m_vertices[index1].normal += n;
        m_vertices[index2].normal += n;
    }

    for (auto& vert : m_vertices)
        vert.normal = math::normalize(vert.normal);
}

void RenderCloth::updateBuffers()
{
    gfx::createVertexArray(m_vao);
    m_vao.numIndices = m_mesh->entries[0].indices.size();
    gfx::bindVertexArray(m_vao);

    m_dbo.init(GL_ARRAY_BUFFER);
    m_dbo.bind();
    m_dbo.setData(sizeof(gfx::Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

    gfx::setVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), 0);
    gfx::setVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, normal));
    gfx::setVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, color));
    gfx::setVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, uv));
    gfx::setVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, tan));
    gfx::setVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, bitan));

    m_ibo.init(GL_ELEMENT_ARRAY_BUFFER);
    m_ibo.bind();
    m_ibo.setData(sizeof(GLuint) * m_mesh->entries[0].indices.size(), &m_mesh->entries[0].indices[0], GL_STATIC_DRAW);

    gfx::unbindVertexArray(m_vao);
    m_dbo.unbind();
    m_ibo.unbind();
}

void RenderCloth::render()
{
    drawVertexArray(m_vao);
}

const gfx::Material* RenderCloth::getMaterial() const
{
    return m_cloth->getMesh()->entries[0].material;
}

RenderEntity::Type RenderCloth::getType() const
{
    return Type::Cloth;
}
