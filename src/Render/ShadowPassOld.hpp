#pragma once
#include <Core/Math3D.hpp>
#include "Cubemap.hpp"
#include "Framebuffer.hpp"
#include "Shader.hpp"
#include "RenderableInstance.hpp"
#include "Light.hpp"
#include <vector>

namespace gfx
{

class ShadowPass
{
public:
    ShadowPass() = default;
    ~ShadowPass() = default;

    void init(std::vector<RenderableInstance::Ptr>* allRenderables);

    void execute(const Light* light[]);

    const Cubemap& getShadowCubemap(u32 index) const;

private:
    struct PointShadow
    {
        Cubemap cubemap;
        Framebuffer fbo;

        mat4 proj;
        mat4 views[6];
    };

    std::vector<RenderableInstance::Ptr>* m_allRenderables = nullptr;

    Shader m_pointShadowShader[Shader::Type::Count];
    PointShadow m_pointShadow[2];
};

}
