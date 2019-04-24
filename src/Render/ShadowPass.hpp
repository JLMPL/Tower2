#pragma once
#include "RenderPass.hpp"
#include "Cubemap.hpp"
#include "Shader.hpp"

namespace gfx
{

class ShadowPass : public RenderPass
{
public:
    ShadowPass() = default;
    ~ShadowPass() = default;

    void init() override final;
    void execute(SceneGraph& graph) override final;

    GLuint getRender() const override final;

private:
    void extractNodes(SceneGraph& graph);

private:
    CameraNode* m_camera = nullptr;
    LightNode* m_light = nullptr;

    Shader m_shadowShader;
    Shader m_ashadowShader;

    Cubemap m_cubemap;
    Framebuffer m_fbo;

    mat4 m_proj;
    mat4 m_views[6];
};

}
