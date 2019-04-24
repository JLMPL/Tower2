#include "PostProcessPass.hpp"

namespace gfx
{

void PostProcessPass::init()
{
    m_bloom.init();
}

void PostProcessPass::execute(GLuint originalRender)
{
    m_originalRender = originalRender;
    m_finalRender = m_bloom.process(m_originalRender);
    m_finalRender = originalRender;
}

GLuint PostProcessPass::getFinalRender() const
{
    return m_finalRender;
}

}
