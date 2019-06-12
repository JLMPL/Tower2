#pragma once
#include "RenderPass.hpp"
#include "Render/Scene/RenderLight.hpp"
#include "Shader.hpp"

#include "LineRenderer.hpp"

namespace gfx
{

class BasePass : public RenderPass
{
public:
    BasePass() = default;
    ~BasePass() = default;

    void init() override final;
    void execute(RenderScene& scene, GLuint shadow0);

    void addLine(const vec3& a, const vec3& b, const vec3& c);

private:
    void extractNodes(RenderScene& scene);
    void sortLights(RenderScene& scene);
    void renderMeshes(RenderScene& scene, GLuint shadow0);

private:
    std::vector<RenderLight*> m_lights;
    RenderLight               m_nullLight;

    gfx::Shader             m_flatShader;
    gfx::Shader             m_aflatShader;
    gfx::Shader             m_flareShader;
    gfx::Shader             m_clothShader;

    LineRenderer            m_lines;
};

}
