#pragma once
#include "Core/Timer.hpp"
#include "Geometry/ScreenQuad.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "BasePass.hpp"
#include "ShadowPass.hpp"
#include <vector>

namespace gfx
{

struct Light;
struct Material;

class Material;
class SkinnedMesh;

class GraphRenderer
{
public:
    GraphRenderer() = default;

    void init();
    void render(SceneGraph& graph);

    void addLine(const vec3& a, const vec3& b, const vec3& c);

private:
    void finalFrame();

private:
    ShadowPass m_shadowPass;
    BasePass   m_basePass;

    Shader     m_frameShader;
    ScreenQuad m_screenQuad;
};

extern GraphRenderer g_GraphRenderer;

}
