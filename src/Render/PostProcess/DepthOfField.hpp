#pragma once
#include "../Geometry/ScreenQuad.hpp"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"
#include "Blur.hpp"

namespace gfx
{

class PostProcessDof
{
public:
    PostProcessDof() = default;
    ~PostProcessDof() = default;

    void init();

    GLuint process(GLuint tex, GLuint depth);

private:
    void blurFar();
    void combine();

private:
    struct Pass
    {
        Framebuffer fbo;
        Shader      shader;
        GLuint      texture;
    };

    ScreenQuad      m_screenQuad;

    GLuint          m_original;
    GLuint          m_depth;

    Pass            m_bokeh;
    Pass            m_combine;
};

}
