#pragma once
#include "Core/Timer.hpp"
#include "Geometry/ScreenQuad.hpp"
#include "Geometry/StaticMesh.hpp"
#include "PostProcess/Bloom.hpp"
#include "Light.hpp"
#include "LineRenderer.hpp"
#include "PostProcessPass.hpp"
#include "RenderableInstance.hpp"
#include "ShadowPassOld.hpp"
#include <vector>

namespace gfx
{

struct Light;
struct Material;

class Camera;
class Material;
class SkinnedMesh;
class HeightField;

class Renderer3D
{
public:
    Renderer3D() = default;

    void init();
    void render();

    void addLine(const vec3& a, const vec3& b, const vec3& c);
    void addAxis(const vec3& pos);
    void addSphere(const vec3& pos, f32 radius, const vec3& color = vec3(1));

    void addLight(const Light* light);
    void removeLight(const Light* light);

    RenderableHandle addFoliage(const StaticMesh::Entry* mesh, const Material* mat);

    RenderableHandle addStatic(const StaticMesh::Entry* mesh,
        const Material* mat = nullptr, bool nonCullable = false, bool nonShadowCasting = false);

    RenderableHandle addSkinned(const SkinnedMesh* mesh, const Material* mat, const mat4* matPalette, u32 mats,
        bool nonCullable = false);

    void removeRenderable(const RenderableHandle& handle);

    void setActiveCamera(const Camera* camera);
    const Camera* getActiveCamera() const;

private:
    i32 addRenderableToMap(RenderableInstance* fi);

    void setPointLights(const Shader& shader);
    void setStandardVertex(
        const Shader& shader, const mat4& proj, const mat4& view, const mat4& model);

    const Shader* bindMaterial(const Material* mat, Shader::Type type, const mat4& model);
    void unbindMaterial();

    bool cullRenderable(const vec3& pos, f32 radius);
    void prepareRenderQueue();
    void renderData();

    void finalFrame();

private:
    const Camera* m_camera = nullptr;

    std::vector<RenderableInstance::Ptr> m_renderables;
    std::vector<RenderableInstance*> m_renderQueue;

    std::vector<const Light*> m_lights;
    Light                     m_nullLight;
    i32                       m_firstEnabledLight = 0;
    const Light*              m_enabledLights[4] = {nullptr};

    LineRenderer              m_lineRenderer;

    ShadowPass                m_shadowPass;
    PostProcessPass           m_postProcessPass;

    Shader                    m_frameShader;

    Framebuffer               m_fbo;
    ScreenQuad                m_screenQuad;
};

extern Renderer3D g_Renderer3D;

}
