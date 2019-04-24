#pragma once
#include "RenderPass.hpp"
#include "SceneGraph/LightNode.hpp"
#include "Shader.hpp"

namespace gfx
{

class BasePass : public RenderPass
{
public:
    BasePass() = default;
    ~BasePass() = default;

    void init() override final;
    void execute(SceneGraph& graph, GLuint shadow0);

private:
    void extractNodes(SceneGraph& graph);
    void sortLights();
    void renderMeshes(SceneGraph& graph, GLuint shadow0);

private:
    CameraNode*             m_cameraNode = nullptr;

    std::vector<LightNode*> m_lights;
    LightNode               m_nullLight;

    gfx::Shader             m_flatShader;
    gfx::Shader             m_aflatShader;

};

}
