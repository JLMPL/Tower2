#pragma once
#include "Render/Scene/RenderScene.hpp"
#include "Framebuffer.hpp"

namespace gfx
{

class RenderPass
{
public:
    RenderPass() = default;
    ~RenderPass() = default;

    virtual void init() {}
    virtual void execute(RenderScene& scene) {}

    virtual GLuint getRender() const;

protected:
    Framebuffer m_fbo;
};

}
