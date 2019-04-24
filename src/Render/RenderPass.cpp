#include "RenderPass.hpp"

namespace gfx
{

GLuint RenderPass::getRender() const
{
    return m_fbo.getTexture();
}

}
