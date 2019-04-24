#pragma once
#include "PostProcess/Bloom.hpp"

namespace gfx
{

class PostProcessPass
{
public:
    PostProcessPass() = default;
    ~PostProcessPass() = default;

    void init();

    void execute(GLuint originalRender);

    GLuint getFinalRender() const;

private:
    GLuint m_originalRender = 0;
    GLuint m_finalRender = 0;

    Bloom m_bloom;
};

}
