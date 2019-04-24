#include "Renderer3D.hpp"
#include "Core/Config.hpp"
#include "Geometry/SkinnedMesh.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Material.hpp"

namespace gfx
{

Renderer3D g_Renderer3D;

void Renderer3D::init()
{
    m_renderables.reserve(1024);

    m_frameShader.loadFromFile("Shaders/Frame.sha");

    m_fbo.init(Framebuffer::Type::Color, 1280, 720);
    m_screenQuad.init();

    m_postProcessPass.init();

    m_lineRenderer.init();

    m_nullLight.pos = vec3(0);
    m_nullLight.color = vec3(0);
    m_nullLight.intensity = 0;

    m_shadowPass.init(&m_renderables);
}

void Renderer3D::addLine(const vec3& a, const vec3& b, const vec3& c)
{
    m_lineRenderer.addLine(a,b,c);
}

void Renderer3D::addAxis(const vec3& pos)
{
    addLine(pos + vec3(1,0,0), pos + vec3(-0.25,0,0), vec3(1,0,0));
    addLine(pos + vec3(0,1,0), pos + vec3(0,-0.25,0), vec3(0,1,0));
    addLine(pos + vec3(0,0,1), pos + vec3(0,0,-0.25), vec3(0,0,1));
}

void Renderer3D::addSphere(const vec3& pos, f32 radius, const vec3& color)
{
    f32 step = M_PI / 32.f;

    for (f32 yaw = 0; yaw < M_TAU; yaw += step)
    {
        addLine(
            pos + vec3(cos(yaw), sin(yaw), 0) * radius,
            pos + vec3(cos(yaw + step), sin(yaw + step), 0) * radius,
            color
        );

        addLine(
            pos + vec3(0, cos(yaw), sin(yaw)) * radius,
            pos + vec3(0, cos(yaw + step), sin(yaw + step)) * radius,
            color
        );

        addLine(
            pos + vec3(cos(yaw), 0, sin(yaw)) * radius,
            pos + vec3(cos(yaw + step), 0, sin(yaw + step)) * radius,
            color
        );
    }
}

void Renderer3D::addLight(const Light* light)
{
    m_lights.push_back(light);
}

void Renderer3D::removeLight(const Light* light)
{
    for (auto i = m_lights.begin(); i != m_lights.end();)
    {
        if (*i == light)
        {
            m_lights.erase(i);
            return;
        }
        else
            i++;
    }
}

RenderableHandle Renderer3D::addFoliage(const StaticMesh::Entry* mesh, const Material* mat)
{
    RenderableInstance::Ptr ren(new RenderableInstance());

    ren->vao[0] = &mesh->vao;
    ren->materials[0] = mat;
    ren->transform = mat4(1.f);
    ren->shaderType = Shader::Type::StaticWavy;
    ren->visible = true;

    m_renderables.push_back(std::move(ren));

    return RenderableHandle(m_renderables.back().get());
}

RenderableHandle Renderer3D::addStatic(const StaticMesh::Entry* mesh, const Material* mat, bool nonCullable, bool nonShadowCasting)
{
    RenderableInstance::Ptr ren(new RenderableInstance());

    ren->vao[0] = &mesh->vao;
    ren->materials[0] = mesh->material;
    ren->transform = mat4(1.f);
    ren->shaderType = Shader::Type::Static;
    ren->visible = true;
    ren->nonCullable = nonCullable;
    ren->nonShadowCasting = nonShadowCasting;

    m_renderables.push_back(std::move(ren));

    return RenderableHandle(m_renderables.back().get());
}

RenderableHandle Renderer3D::addSkinned(
    const SkinnedMesh* mesh, const Material* mat, const mat4* matPalette, u32 numJoints,
    bool nonCullable)
{
    RenderableInstance::Ptr ren(new RenderableInstance());

    for (u32 i = 0; i < mesh->entries.size(); i++)
    {
        if (mat)
            ren->materials[i] = mat;
        else
            ren->materials[i] = mesh->entries[i].material;

        ren->vao[i] = &mesh->entries[i].vao;
    }

    ren->transform = mat4(1.f);
    ren->shaderType = Shader::Type::Skinned;
    ren->visible = true;
    ren->matrixPalette = matPalette;
    ren->numJoints = numJoints;
    ren->nonCullable = nonCullable;

    m_renderables.push_back(std::move(ren));

    return RenderableHandle(m_renderables.back().get());
}

void Renderer3D::removeRenderable(const RenderableHandle& handle)
{

}

void Renderer3D::setPointLights(const Shader& shader)
{
    shader.setUniformTexture("uPointShadowCubemap[0]", 3, m_shadowPass.getShadowCubemap(0));
    shader.setUniformTexture("uPointShadowCubemap[1]", 4, m_shadowPass.getShadowCubemap(1));

    if (m_lights.empty())
        return;

    for (u32 i = 0; i < 4; i++)
    {
        std::string index = std::to_string(i);

        std::string pos = "uPointLights[" + index + "].pos";
        std::string color = "uPointLights[" + index + "].color";
        std::string intensity = "uPointLights[" + index + "].intensity";

        shader.setUniform3f(pos.c_str(), m_enabledLights[i]->pos);
        shader.setUniform3f(color.c_str(), m_enabledLights[i]->color);
        shader.setUniform1f(intensity.c_str(), m_enabledLights[i]->intensity);
    }
}

void Renderer3D::setStandardVertex(
    const Shader& shader,
    const mat4& proj,
    const mat4& view,
    const mat4& model
    )
{
    shader.setUniformMatrix("uProj", proj);
    shader.setUniformMatrix("uView", view);
    shader.setUniformMatrix("uModel", model);
    shader.setUniform3f("uCamPos", m_camera->getPos());
}

const Shader* Renderer3D::bindMaterial(
    const Material* mat,
    Shader::Type type,
    const mat4& model
    )
{
    const Shader* shader = mat->m_shader[type];

    shader->bind();
    setStandardVertex(*shader, m_camera->getProjection(), m_camera->getView(), model);

    shader->setUniformTexture("uImage", 0, *mat->m_textures[0]);
    shader->setUniformTexture("uNormal", 1, *mat->m_textures[1]);
    shader->setUniformTexture("uSpecular", 2, *mat->m_textures[2]);

    setPointLights(*shader);

    return shader;
}

void Renderer3D::unbindMaterial()
{
    m_frameShader.unbind();
    GL(glDisable(GL_BLEND));
}

bool Renderer3D::cullRenderable(const vec3& pos, f32 radius)
{
    vec4 furpos = vec4(pos, 1);
    furpos = m_camera->getProjection() * m_camera->getView() * furpos;
    furpos.x /= furpos.w;
    furpos.y /= furpos.w;
    furpos.z /= furpos.w;
    radius /= furpos.w;

    if (furpos.x + radius < -1 || furpos.x - radius > 1)
        return false;
    if (furpos.y + radius < -1 || furpos.y - radius > 1)
        return false;
    if (furpos.z + radius < -1 || furpos.z - radius > 1)
        return false;

    return true;
}

void Renderer3D::prepareRenderQueue()
{
    m_renderQueue.clear();

    for (u32 i = 0; i < m_renderables.size(); i++)
    {
        if (m_renderables[i])
        {
            if (m_renderables[i]->visible &&
                (cullRenderable(vec3(m_renderables[i]->transform[3]), 1.f) || m_renderables[i]->nonCullable))
            {
                m_renderQueue.push_back(m_renderables[i].get());
            }
        }
    }
}

void Renderer3D::renderData()
{
    m_fbo.bind();
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    for (u32 i = 0; i < m_renderQueue.size(); i++)
    {
        for (u32 j = 0; j < 8; j++)
        {
            if (!m_renderQueue[i]->vao[j])
                continue;

            auto shader = bindMaterial(
                m_renderQueue[i]->materials[j],
                m_renderQueue[i]->shaderType,
                m_renderQueue[i]->transform
            );

            if (m_renderQueue[i]->matrixPalette)
            {
                for (u32 j = 0; j < m_renderQueue[i]->numJoints; j++)
                {
                    char index[10];
                    sprintf(index, "bones[%d]", j);
                    shader->setUniformMatrix(index, m_renderQueue[i]->matrixPalette[j]);
                }
            }

            m_renderQueue[i]->vao[j]->drawElements();

            unbindMaterial();
        }
    }

    m_lineRenderer.render(m_camera->getView(), m_camera->getProjection());

    m_fbo.unbind();
}

void Renderer3D::finalFrame()
{
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_frameShader.bind();
    m_frameShader.setUniformTexture("image", 0, m_postProcessPass.getFinalRender());
    m_screenQuad.render();
    m_frameShader.unbind();
}

void Renderer3D::render()
{
    m_lineRenderer.prepare();

    u32 lightIndex = 0;

    for (u32 i = 0; i < 4; i++)
    {
        m_enabledLights[i] = &m_nullLight;
    }

    for (u32 i = 0; i < m_lights.size() + 4; i++)
    {
        if (m_lights.size() <= i)
            break;

        if (m_lights[i]->enabled)
        {
            m_firstEnabledLight = i;
            m_enabledLights[lightIndex++] = m_lights[i];
        }

        if (lightIndex >= 4)
                break;
    }

    m_shadowPass.execute(m_enabledLights);

    prepareRenderQueue();
    renderData();

    m_postProcessPass.execute(m_fbo.getTexture());

    finalFrame();

    m_firstEnabledLight = 0;
    m_lineRenderer.clear();
}

void Renderer3D::setActiveCamera(const Camera* camera)
{
    m_camera = camera;
}

const Camera* Renderer3D::getActiveCamera() const
{
    return m_camera;
}

}
