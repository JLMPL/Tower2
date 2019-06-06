#include "ClothNode.hpp"
#include "Render/Geometry/StaticMesh.hpp"
#include "Physics/Cloth.hpp"

ClothNode::ClothNode(phys::Cloth* cloth) :
    m_cloth(cloth)
{
    m_mesh = m_cloth->getMesh();

    m_vertices.resize(m_mesh->entries[0].vertices.size());

    for (auto i = 0; i < m_vertices.size(); i++)
    {
        m_vertices[i] = m_mesh->entries[0].vertices[i];
    }
}

void ClothNode::updateGeometry()
{
    updatePositions();
    // updateNormals();
    updateBuffers();
}

void ClothNode::updatePositions()
{
    m_cloth->lockParticleData();
    for (auto i = 0; i < m_cloth->getVertexCount(); i++)
    {
        m_vertices[i].pos = m_cloth->getVertices()[i];
    }
    m_cloth->unlockParticleData();
}

void ClothNode::updateNormals()
{
    for (auto& vert : m_vertices)
        vert.normal = vec3(0);

    for (auto i = 0; i < m_mesh->entries[0].indices.size(); i += 3)
    {
        i32 index = m_mesh->entries[0].indices[i];
        vec3 a = m_vertices[index].pos;
        vec3 b = m_vertices[index + 1].pos;
        vec3 c = m_vertices[index + 2].pos;

        vec3 n = math::normalize(math::cross(b - a, c - a));

        m_vertices[index].normal += n;
        m_vertices[index + 1].normal += n;
        m_vertices[index + 2].normal += n;
    }

    for (auto& vert : m_vertices)
        vert.normal = math::normalize(vert.normal);
}

void ClothNode::updateBuffers()
{
    m_vao.init();
    m_vao.setIndexNumber(m_mesh->entries[0].indices.size());
    m_vao.bind();

    m_dbo.init(GL_ARRAY_BUFFER);
    m_dbo.bind();
    m_dbo.setData(sizeof(gfx::Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

    m_vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), 0);
    m_vao.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, normal));
    m_vao.vertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, color));
    m_vao.vertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, uv));
    m_vao.vertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, tan));
    m_vao.vertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex), (const GLvoid*)offsetof(gfx::Vertex, bitan));

    m_ibo.init(GL_ELEMENT_ARRAY_BUFFER);
    m_ibo.bind();
    m_ibo.setData(sizeof(GLuint) * m_mesh->entries[0].indices.size(), &m_mesh->entries[0].indices[0], GL_STATIC_DRAW);

    m_vao.unbind();
    m_dbo.unbind();
    m_ibo.unbind();
}

void ClothNode::render() const
{
    m_vao.drawElements();
}

const gfx::Material* ClothNode::getMaterial() const
{
    return m_cloth->getMesh()->entries[0].material;
}

SceneNode::Type ClothNode::getType() const
{
    return Type::Cloth;
}
