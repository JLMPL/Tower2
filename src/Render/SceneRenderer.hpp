#pragma once
#include "Core/Timer.hpp"
#include "Geometry/ScreenQuad.hpp"
#include "Render/Scene/RenderScene.hpp"
#include "BasePass.hpp"
#include "ShadowPass.hpp"
#include <vector>

namespace gfx
{

struct Light;
struct Material;

class Material;
class Mesh;

class SceneRenderer
{
public:
    SceneRenderer() = default;

    void init();
    void render(RenderScene& scene);

    void addLine(const vec3& a, const vec3& b, const vec3& c);
    void addSphere(const vec3& pos, f32 rad, const vec3& c);

private:
    void finalFrame();

private:
    ShadowPass m_shadowPass;
    BasePass   m_basePass;

    Shader     m_frameShader;
    ScreenQuad m_screenQuad;
};

extern SceneRenderer g_SceneRenderer;

}
