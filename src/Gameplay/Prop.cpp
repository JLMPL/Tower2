#include "Prop.hpp"
#include "Render/MaterialManager.hpp"

void Prop::init(const gfx::StaticMesh* mesh, const mat4* tr, bool hasLight)
{
    setParentTransform(tr);
    m_localTransform = mat4(1);

    m_hasLight = hasLight;

    if (m_hasLight)
    {
        m_light.pos = vec3(10,2,0);
        m_light.color = vec3(1);
        m_light.intensity = 20.f;
        m_light.enabled = false;

        gfx::g_Renderer3D.addLight(&m_light);
    }

    m_mesh = gfx::g_Renderer3D.addStatic(&mesh->entries[0], gfx::g_MatMgr.getMaterial("env_wood"), false);
    m_mesh.hide();
}

void Prop::updateTransforms()
{
    if (m_parent)
        m_transform = (*m_parent) * m_localTransform;
    else
        m_transform = m_localTransform;

    m_light.pos = vec3(m_transform * vec4(0,0.5,0,1));

    m_mesh.setTransform(m_transform);
}

void Prop::setMesh(const gfx::StaticMesh* mesh)
{
    m_mesh.setMesh(mesh->entries[0]);
}

void Prop::setLocalTransform(const mat4& tr)
{
    m_localTransform = tr;
}

void Prop::setParentTransform(const mat4* tr)
{
    m_parent = tr;
}

void Prop::enableLight()
{
    m_light.enabled = true;
}

void Prop::disableLight()
{
    m_light.enabled = false;
}

void Prop::show()
{
    m_mesh.show();

    if (m_hasLight)
        m_light.enabled = true;
}

void Prop::hide()
{
    m_mesh.hide();
    m_light.enabled = false;
}
