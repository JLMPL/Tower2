#pragma once
#include "SceneGraph/SceneGraph.hpp"
#include "Framebuffer.hpp"

namespace gfx
{

class RenderPass
{
public:
    RenderPass() = default;
    ~RenderPass() = default;

    virtual void init() {}
    virtual void execute(SceneGraph& graph) {}

    virtual GLuint getRender() const;

protected:
    Framebuffer m_fbo;
};

}
